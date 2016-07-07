/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : obrep2.c - datenrepräsentationsspezifisch
 *
 * $Revision: 1.3 $
 * $Log: obrep2.c,v $
 * Revision 1.3  1993/11/04  14:05:11  sma
 * Fehler in save_form2 gehoben
 *
 * Revision 1.2  1993/10/29  15:21:23  sma
 * Fehlerkorrekturen.
 *
 * Revision 1.1  1993/10/14  15:43:26  sma
 * Initial revision
 *
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#if __OBREP == 2

#define T_FORWARD 55
#define LOAD_FORWARD(adr,loc)  ((loc)[0] = T_FORWARD, (loc)[1] = (long)(adr))
#define SAVE_DIMS(fptr) \
   AR_DIMS(fptr) = fx_swap(AR_DIMS(fptr), AR_RANK(fptr) + 1)

TAG type_of();


/*------------------------------------------------------------------------------
 * globale Heap-Variablen
 *----------------------------------------------------------------------------*/
extern CL_FORM *fo_heap;
extern CL_FORM *form_toh;
extern CL_FORM *old_form_heap;
extern unsigned form_heapsize;
extern long *fx_heap1;
extern long *fx_toh;
extern unsigned fx_heapsize;
extern double *fl_heap1;
extern double *fl_toh;
extern double *fl_eoh;
extern unsigned fl_heapsize;


/*------------------------------------------------------------------------------
 * Kopiere "form" rekursiv in anderen Heap.
 *----------------------------------------------------------------------------*/
void save_form2 (form) 
CL_FORM *form;
{
   long f, *fptr;

 AGAIN:
   /* die untersten Bits von f enthalten TAG info */
   f = *form;
   
   switch (f & 7)
   {
   case 0:
      if (f == 0)               /* Unbound */
         break;

      if (f == NIL_VALUE)       /* NIL */
         break;

      /* f ist ein Zeiger, dereferenzieren */
      fptr = (long *)f;
   
      if (fptr < fo_heap)       /* konstantes Datum */
         break;

      if (fptr >= fo_heap + 2 * form_heapsize)
         Labort("Unexpected pointer out of heap");

      if (!(old_form_heap <= fptr && fptr < old_form_heap + form_heapsize))
         Labort("Unexpected pointer in wrong heap.");

      if ((long)fptr & 7)
         Labort("Unaligned ptr.");
 
      f = *fptr;

      if (f == T_FORWARD)       /* Forward-Referenz */
      {
         *form = fptr[1];
         break;
      }

      if ((f & 1) == 0)         /* CONS-Zelle */
      {
         COPY(fptr, form_toh);
         COPY(fptr + 1, form_toh + 1);

         *form = (long)form_toh;
         
         LOAD_FORWARD(form_toh, fptr);

         fptr = form_toh;
         form_toh += 2;
         
         /* CAR kopieren, wenn kein immediate-Wert */
         if ((f & 7) == 0)
            save_form2(fptr);

         /* CDR kopieren, vermeide unnötige Tail-End-Rekursion */
         form = fptr + 1;
         goto AGAIN;
      }

      switch (f & 127)          /* Rest */
      {
      case T_IND:
         COPY(fptr, form_toh);
         COPY(fptr + 1, form_toh + 1);
         
         *form = (long)form_toh;

         /* LOAD_FORWARD unnötig, da immer nur genau ein Zeiger auf
          * einen T_IND existiert */

         form_toh += 2;

         form = form_toh - 1;
         goto AGAIN;
         
      case T_SYMBOL:            /* Symbol */
         *form = (long)form_swap(fptr, 6);

         LOAD_FORWARD(*form, fptr);

         fptr = (long *)*form;
         save_form2(fptr + 2);
         save_form2(fptr + 3);
         save_form2(fptr + 4);
         
         goto smstr;
         
      case T_SMSTR:
         COPY(fptr, form_toh);
         COPY(fptr + 1, form_toh + 1);
         
         *form = (long)form_toh;

         LOAD_FORWARD(form_toh, fptr);
         
         fptr = form_toh;
         form_toh += 2;

      {
         long *str;
      smstr:
         str = (long *)fptr[1];

         if (str >= fx_heap1 && str < fx_heap1 + 2 * fx_heapsize)
         {
            fptr[1] = (long)ch_swap((char *)str, fptr[0] >> 8);
         }
         break;
      }
       
      case T_SMVEC_T:
      case T_STRUCT:
      case T_INSTANCE:
         f = f >> 8;
         *form = (long)form_swap(fptr, f + 1);
         
         LOAD_FORWARD(*form, fptr);
         
         fptr = (long *)*form + 1;
         while (--f > 0)
            save_form2(fptr++);
            
         form = fptr;
         goto AGAIN;
         
      case T_CLOSURE:
         f = f >> 8;
         *form = (long)form_swap(fptr, f + 1);
         
         LOAD_FORWARD(*form, fptr);
         
         fptr = (long *)*form + 2;  /* code ueberspringen */
         f--;
         while (--f > 0)
            save_form2(fptr++);
            
         form = fptr;
         goto AGAIN;

      case T_CFILE:
      case T_GLOBFUN:
      case T_DOWNFUN:
         *form = (long)form_swap(fptr, 2);
         
         LOAD_FORWARD(*form, fptr);
         
         return;

      case T_SMVEC_FIXNUM:
         *form = (long)form_swap(fptr, 2);
         LOAD_FORWARD(*form, fptr);
         fptr = (long *)*form;
         FIXNUM_AR(fptr) = fx_swap(FIXNUM_AR(fptr), AR_SIZE(fptr));
         return;

      case T_SMVEC_FLOAT:
         *form = (long)form_swap(fptr, 2);
         LOAD_FORWARD(*form, fptr);
         fptr = (long *)*form;
         FLOAT_AR(fptr) = fl_swap(FLOAT_AR(fptr), AR_SIZE(fptr));
         return;

      case T_VEC_T:
      case T_VEC_FIXNUM:
      case T_VEC_FLOAT:
      case T_STRING:
         *form = (long)(form_swap(fptr - 2, 4L) + 2);
         LOAD_FORWARD(*form, fptr);
         fptr = (long *)*form;
         if (DISPLACED_P(fptr))
         {
            save_form2(DISPLACED_TO(fptr));
         }
         else
         {
            long sz;
            sz = HAS_FILL_PTR(fptr) ? AR_SIZE_WHEN_FP(fptr) : AR_SIZE(fptr);
            switch (f & 127)
            {
            case T_VEC_T:
               FORM_AR(fptr) = form_swap(FORM_AR(fptr), sz);
               fptr = FORM_AR(fptr);
               while (--sz > 0)
                  save_form2(fptr++);
               form = fptr;
               goto AGAIN;
            case T_VEC_FIXNUM:
               FIXNUM_AR(fptr) = fx_swap(FIXNUM_AR(fptr), sz);
               break;
            case T_VEC_FLOAT:
               FLOAT_AR(fptr) = fl_swap(FLOAT_AR(fptr), sz);
               break;
            case T_STRING:
               CHAR_AR(fptr) = ch_swap(CHAR_AR(fptr), sz);
               break;
            }
         }
         return;

      case T_AR_T:
      case T_AR_FIXNUM:
      case T_AR_FLOAT:
      case T_AR_CHAR:
         *form = (long)(form_swap(fptr - 2, 4) + 2);
         LOAD_FORWARD(*form, fptr);
         fptr = (long *)*form;
         SAVE_DIMS(fptr);
         if (DISPLACED_P(fptr))
         {
            save_form2(DISPLACED_TO(fptr));
         }
         else
         {
            long sz;
            sz = HAS_FILL_PTR(fptr) ? AR_SIZE_WHEN_FP(fptr) : AR_SIZE(fptr);
            switch (f & 127)
            {
            case T_AR_T:
               FORM_AR(fptr) = form_swap(FORM_AR(fptr), sz);
               fptr = FORM_AR(fptr);
               while (--sz > 0)
                  save_form2(fptr++);
               form = fptr;
               goto AGAIN;
            case T_AR_FIXNUM:
               FIXNUM_AR(fptr) = fx_swap(FIXNUM_AR(fptr), sz);
               break;
            case T_AR_FLOAT:
               FLOAT_AR(fptr) = fl_swap(FLOAT_AR(fptr), sz);
               break;
            case T_AR_CHAR:
               CHAR_AR(fptr) = ch_swap(CHAR_AR(fptr), sz);
               break;
            }
         }
         return;

      case T_SMAR_T:
         f = f >> 8;
         *form = (long)(form_swap(fptr - 1, f + 2) + 1);
         LOAD_FORWARD(*form, fptr);
         fptr = (long *)*form;
         SAVE_DIMS(fptr);

         fptr++;
         while (--f > 0)
            save_form2(fptr++);
            
         form = fptr;
         goto AGAIN;

      case T_SMAR_FIXNUM:
         *form = (long)(form_swap(fptr - 1, 3) + 1);
         LOAD_FORWARD(*form, fptr);
         fptr = GET_FORM(form);
         SAVE_DIMS(fptr);
         FIXNUM_AR(fptr) = fx_swap(FIXNUM_AR(fptr), AR_SIZE(fptr));
         return;

      case T_SMAR_FLOAT:
         *form = (long)(form_swap(fptr - 1, 3) + 1);
         LOAD_FORWARD(*form, fptr);
         fptr = GET_FORM(form);
         SAVE_DIMS(fptr);
         FLOAT_AR(fptr) = fl_swap(FLOAT_AR(fptr), AR_SIZE(fptr));
         return;

      case T_SMAR_CHAR:
         *form = (long)(form_swap(fptr - 1, 3) + 1);
         LOAD_FORWARD(*form, fptr);
         fptr = GET_FORM(form);
         SAVE_DIMS(fptr);
         CHAR_AR(fptr) = ch_swap(CHAR_AR(fptr), AR_SIZE(fptr));
         return;
         
      case T_C_FOREIGN:
         Labort("sma-error: foreign function");

      default:
         Labort("sma-error, unbekannter TAG, Stufe 2");
      }
      break;

   case 2:                      /* Fixnum */
   case 6:                      /* Char */
      break;
      
   case 4:                      /* Float */
   {
      double *dptr = (double *)(f - 4);
      if (dptr >= fl_heap1 && dptr < fl_heap1 + 2 * fl_heapsize)
      {
         *fl_toh = *dptr;
         *form = (long)fl_toh + 4;
         if (++fl_toh >= fl_eoh)
            Labort("Float-Heap Overflow.");
      }
      else if (!(dptr < fl_heap1))
         Labort("sma-error, wrong float");
         
      break;
   }
      
   default:
      Labort("sma-error, unbekannter Tag, Stufe 1");
   }
}


/*------------------------------------------------------------------------------
 * Reserviert Speicher, der auf einer 8-Byte-Grenze liegt.
 *----------------------------------------------------------------------------*/
PTR malloc8(size)
unsigned size;
{
   PTR mem = malloc(size + 7);

   /* wenn mem==NULL, dann auch (NULL+7)&~7==NULL */
   return (PTR)(((long)mem + 7) & ~7);
}


/*------------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/
TAG type_of(form)
CL_FORM *form;
{
   long f, *fptr;
   
   f = *form;
   
   switch (f & 7)
   {
   case 0:
      if (f == 0)
         return CL_UNBOUND;
      
      if (f == NIL_VALUE)
         return CL_NIL;
      
      fptr = (long *)f;
      f = *fptr;

      if ((f & 1) == 0)
         return CL_CONS;
      
      switch (f & 127)
      {
      case T_SYMBOL:  
         return CL_SYMBOL;

      case T_VEC_T: 
         return CL_VEC_T;
      case T_VEC_FIXNUM:
         return CL_VEC_FIXNUM;
      case T_VEC_FLOAT:
         return CL_VEC_FLOAT;
      case T_STRING:
         return CL_STRING;

      case T_SMVEC_T: 
         return CL_SMVEC_T;
      case T_SMVEC_FIXNUM:
         return CL_SMVEC_FIXNUM;
      case T_SMVEC_FLOAT:
         return CL_SMVEC_FLOAT;
      case T_SMSTR:
         return CL_SMSTR;

      case T_SMAR_T:
         return CL_SMAR_T;
      case T_SMAR_FIXNUM:
         return CL_SMAR_FIXNUM;
      case T_SMAR_FLOAT:
         return CL_SMAR_FLOAT;
      case T_SMAR_CHAR:
         return CL_SMAR_CHAR;
         
      case T_AR_T:
         return CL_AR_T;
      case T_AR_FIXNUM:
         return CL_AR_FIXNUM;
      case T_AR_FLOAT:
         return CL_AR_FLOAT;
      case T_AR_CHAR:
         return CL_AR_CHAR;
         
      case T_INSTANCE:
         return CL_INSTANCE;
      case T_STRUCT:
         return CL_STRUCT;

      case T_CLOSURE:
         return CL_CLOSURE;
      case T_GLOBFUN:
         return CL_GLOBFUN;
      case T_DOWNFUN:
         return CL_DOWNFUN;
         
      case T_CFILE:
         return CL_CFILE;
      }
      break;
      
   case 2:
      return CL_FIXNUM;
      
   case 4:
      return CL_FLOAT;

   case 6:
      return CL_CHAR;
   }
   Labort("sma-error: TYPE_OF-error");
}


/*------------------------------------------------------------------------------
 * Spezielle Testfunktionen für (eq form #'eq) bzw. #'eql
 *----------------------------------------------------------------------------*/

#define IS_FUNCTION_EQ(form) (CL_CLOSUREP(form) && GET_GFARG(form) == Feq)
#define IS_FUNCTION_EQL(form) (CL_CLOSUREP(form) && GET_GFARG(form) == Feql)

#endif
