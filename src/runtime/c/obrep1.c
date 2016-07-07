/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : obrep1.c - datenrepräsentationsspezifisch
 *
 * $Revision: 1.2 $
 * $Log: obrep1.c,v $
 * Revision 1.2  1993/10/29  15:19:41  sma
 * Änderung wegen neuer Verwaltung von Array-Dimensionen.
 *
 * Revision 1.1  1993/10/14  15:42:33  sma
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

#if __OBREP == 1

#define GC_FORWARD    128
#define LOAD_FORWARD(fw_ptr, loc) \
   (SET_TAG (loc, GC_FORWARD), GET_FORM (loc) = (fw_ptr))


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
extern double *old_fl_heap;
extern unsigned fl_heapsize;


/*------------------------------------------------------------------------------
 * Rettet eine LISP-Form, falls noetig, in den neuen Heap.
 *----------------------------------------------------------------------------*/
void save_form (form)
CL_FORM *form;
{
 begin:
   switch (TYPE_OF (form))
   {
   case CL_UNBOUND:
   case CL_FIXNUM:
   case CL_CHAR:
   case CL_NIL:
   case CL_DOWNFUN:
   case CL_GLOBFUN:
   case CL_CODE:
   case CL_CFILE:
   case CL_UNIQUE_TAG:
      break;
      
   /*------------------------------------------------------------------------   
    * Garbage-Collection eines LISP-Datums,                                  
    * welches durch einen Zeiger referenziert wird.                          
    * Annahme: Konstanten des Typs 'double' und 'CL_FORM', die schon zur     
    * Uebersetzungszeit in 'floats' bzw 'const_forms' angelegt wurden,       
    * haben Adressen, die kleiner sind als die Adresse von Daten, die        
    * erst zur Laufzeit angelegt wurden                                      
    *----------------------------------------------------------------------*/
   case CL_FLOAT:
   {
      double *lptr = GET_FLOAT_PTR(form);

      if(lptr < fl_heap1)       /* Konstantes Datum */
         return;
      if (lptr >= fl_heap1 + 2 * fl_heapsize)
         Labort ("Unexpected pointer out of heap.");
      if (!(old_fl_heap <= lptr && lptr < old_fl_heap + fl_heapsize))
         Labort ("Unexpected pointer in wrong heap.");

      /* ACHTUNG: wenn mehrere Referenzen auf eine Fließkommazahl existieren, */
      /* dann wird die Zahl mehrmals kopiert! --> evtl. Überlauf während */
      /* der GC */
      
      *fl_toh = *lptr;
      GET_FLOAT_PTR(form) = fl_toh++;
      if(fl_toh >= fl_eoh)
         Labort("Float-Heap Overflow.");
      break;
   }
   default:
   {
      CL_FORM *lptr = GET_FORM (form);
      
      /*------------------------------------------------------------------------
       * Annahme:
       * - Der initialisierte Konstantenbereich wird bereits zur
       *   Übersetzungszeit alloziert.
       * - Nicht initialisierte Arrays werden erst zur Laufzeit alloziert.
       * - Zur Laufzeit allozierte Speicherbereiche liegen adressenmaessig
       *   oberhalb des Programms.
       * 
       * => Zur Ueberpruefung, ob der Zeiger nicht in den Heap zeigt,
       * reicht die Abfrage:
       *----------------------------------------------------------------------*/
      if (lptr < fo_heap)       /* Konstantes Datum */
         return;
      if (lptr >= fo_heap + 2 * form_heapsize)
         Labort ("Unexpected pointer out of heap.");
      if (!(old_form_heap <= lptr && lptr < old_form_heap + form_heapsize))
         Labort ("Unexpected pointer in wrong heap.");

      /* Wenn Vorwärtsreferenz, dann Datum bereits kopiert.
       * Neue Adresse eintragen und fertig. */
      if (TYPE_OF(lptr) == GC_FORWARD)
      {
         GET_FORM(form) = GET_FORM(lptr);
         return;
      }
      
      switch (TYPE_OF (form)) {

      /* Garbage-Collection von Symbolen */
      /*---------------------------------*/
      case CL_SYMBOL:
         GET_FORM(form) = form_swap(lptr, SYM_SIZE);
         LOAD_FORWARD(GET_FORM(form), lptr);

         lptr = GET_FORM(form); /* Zeiger auf das neue Symbol */
         
         /* SYMBOL Printnamen retten, falls String im Character-Heap liegt.
          * MAKE-SYMBOL kopiert bei einem SIMPLE-STRING nicht den
          * eigentlichen String! Deshalb kann ein SYMBOL im Heap als
          * Printnamen auch einen C-String haben. */
         if (CHAR_AR(lptr) >= (char *)fx_heap1)
            CHAR_AR(lptr) = ch_swap(CHAR_AR(lptr), AR_SIZE(lptr));
         
         save_form(lptr + OFF_SYM_PLIST);
         save_form(lptr + OFF_SYM_VALUE);
         save_form(lptr + OFF_SYM_PACKAGE);
         break;

      /* Garbage-Collection von Listen */
      /*------------------------------ */
      case CL_CONS:
         COPY(lptr, form_toh);
         COPY(lptr + 1, form_toh + 1);
         
         /* Zeiger auf kopierten CONS-Knoten setzen */
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, lptr);
         form_toh += 2;

         /* Depth-First Strategie: Zuerst den CAR weiterverfolgen */
         save_form (GET_FORM (form));

         /* Dann den CDR weiterverfolgen.
          * Hier wird 'form' neu gesetzt und durch einen Sprung an den
          * Anfang der Funktion unnoetige Rekursion vermieden. */
         form = GET_FORM (form) + 1;
         goto begin;
         break;

      /* Bei einem VECTOR wird
       * 1. Der Header kopiert, der Zeiger auf den neuen Header gesetzt und
       *    im alten Header eine Vorwärtsreferenz auf den neuen Header
       *    eingetragen. (Der Header beginnt 2 Einträge vor 'lptr' und
       *    ist 4 Eintraege lang.)
       * 2. Falls es sich um einen DISPLACED-TO Vektor handelt, wird der
       *    Vektor, auf welchen referenziert wird, gerettet.
       * 3. Sonst wird die Länge des Vektors bestimmt,
       *    (FILL-POINTER wird ignoriert)
       *    und die Elemente werden kopiert. */
      /*-------------------------------------*/
      case CL_VEC_T:
      case CL_VEC_FIXNUM:
      case CL_VEC_FLOAT:
      case CL_STRING:
         GET_FORM(form) = form_swap(lptr - 2, 4L) + 2;
         LOAD_FORWARD(GET_FORM(form), lptr);

         lptr = GET_FORM (form); /* Zeiger auf den neuen Header */
         if (DISPLACED_P (lptr))
         {
            save_form (DISPLACED_TO (lptr));
         }
         else
         {
            long i, size;

            size = HAS_FILL_PTR (lptr) ? AR_SIZE_WHEN_FP(lptr) : AR_SIZE(lptr);
            switch (TYPE_OF (form)) {
            case CL_VEC_T:
               FORM_AR(lptr) = form_swap(FORM_AR(lptr), size);
               for (lptr = FORM_AR(lptr), i = 0; i < size; i++)
                  save_form(lptr + i);
               break;
            case CL_VEC_FIXNUM:
               FIXNUM_AR(lptr) = fx_swap(FIXNUM_AR(lptr), size);
               break;
            case CL_VEC_FLOAT:
               FLOAT_AR(lptr) = fl_swap(FLOAT_AR(lptr), size);
               break;
            case CL_STRING:
               CHAR_AR(lptr) = ch_swap(CHAR_AR(lptr), size);
            }
         }
         break;

      /* Bei einem SIMPLE-VECTOR wird:
       * 1. Der Header kopiert.
       * 2. Der eigentliche Vektor kopiert.
       * 3. Im alten Header eine Vorwaertsreferenz auf den neuen Header
       *     eingetragen.
       * 4. Falls die Elemente des Vektors wieder CL_FORM's sind,
       *    das GC auf die Elemente ausgedehnt. */
      /*----------------------------------------*/
      case CL_SMVEC_T:
      case CL_CLOSURE:
      case CL_STRUCT:
      case CL_INSTANCE:
      {
         long i, size = AR_SIZE (lptr);

         GET_FORM(form) = form_swap(lptr, 1 + size);
         LOAD_FORWARD(GET_FORM(form), lptr);
         for (lptr = GET_FORM(lptr) + 1, i = 0; i < size; i++)
            save_form (lptr + i);
         break;
      }
      case CL_SMVEC_FIXNUM:
         GET_FORM (form) = form_swap (lptr, 2L);
         LOAD_FORWARD (GET_FORM (form), lptr);
         lptr = GET_FORM (form); /* Zeiger auf den neuen Header */
         FIXNUM_AR (lptr) = fx_swap (FIXNUM_AR (lptr), AR_SIZE (lptr));
         break;
      case CL_SMVEC_FLOAT: 
         GET_FORM (form) = form_swap (lptr, 2L);
         LOAD_FORWARD (GET_FORM (form), lptr);
         lptr = GET_FORM (form); /* Zeiger auf den neuen Header */
         FLOAT_AR (lptr) = fl_swap (FLOAT_AR (lptr), AR_SIZE (lptr));
         break;
      case CL_SMSTR:    
         GET_FORM (form) = form_swap (lptr, 2L);
         LOAD_FORWARD (GET_FORM (form), lptr);
         lptr = GET_FORM (form); /* Zeiger auf den neuen Header */
         CHAR_AR (lptr)  = ch_swap (CHAR_AR (lptr), AR_SIZE (lptr));
         break;

      /* Simple Arrays */
      /*---------------*/
      case CL_SMAR_T:  
      {
         long i, size = AR_SIZE(lptr);

         GET_FORM(form) = form_swap(lptr - 1, 2L + size) + 1;
         LOAD_FORWARD(GET_FORM (form), lptr);

         lptr = GET_FORM (form); /* Zeiger auf den neuen Header */
         AR_DIMS (lptr) = fx_swap (AR_DIMS (lptr), AR_RANK (lptr) + 1);

         lptr++;                /* Zeiger auf das eigentliche Array */
         for (i = 0; i < size; i++)
            save_form (lptr + i);
         break;
      }
      
      case CL_SMAR_FIXNUM:
      case CL_SMAR_FLOAT:
      case CL_SMAR_CHAR:
         GET_FORM (form) = form_swap (lptr - 1, 3L) + 1;
         LOAD_FORWARD (GET_FORM (form), lptr);

         lptr = GET_FORM (form); /* Zeiger auf den neuen Header */
         AR_DIMS (lptr) = fx_swap (AR_DIMS (lptr), AR_RANK (lptr) + 1);
         switch (TYPE_OF (form)) {
         case CL_SMAR_FIXNUM:
            FIXNUM_AR (lptr) = fx_swap (FIXNUM_AR (lptr), AR_SIZE (lptr));
            break;
         case CL_SMAR_FLOAT: 
            FLOAT_AR (lptr) = fl_swap (FLOAT_AR (lptr), AR_SIZE (lptr));
            break;
         case CL_SMAR_CHAR:  
            CHAR_AR (lptr)  = ch_swap (CHAR_AR (lptr), AR_SIZE (lptr));
         }
         break;

      /* Arrays */
      /*--------*/
      case CL_AR_T:
      case CL_AR_FIXNUM:
      case CL_AR_FLOAT:
      case CL_AR_CHAR:
         GET_FORM (form) = form_swap (lptr - 2, 4L) + 2;
         LOAD_FORWARD (GET_FORM (form), lptr);

         lptr = GET_FORM (form); /* Zeiger auf den neuen Header */
         AR_DIMS (lptr) = fx_swap (AR_DIMS (lptr), AR_RANK (lptr) + 1);
         if (DISPLACED_P (lptr))
         {
            save_form (DISPLACED_TO (lptr));
         }
         else
         {
            switch (TYPE_OF (form))
            {
            case CL_AR_T:
            {
               long i, size = AR_SIZE (lptr);
               
               FORM_AR (lptr) = form_swap (FORM_AR (lptr), size);
               
               /* Zeiger auf das eigentliche Array */
               for (lptr = FORM_AR (lptr), i = 0; i < size; i++)
                  save_form (lptr + i);
               break;
            }
            case CL_AR_FIXNUM:
               FIXNUM_AR (lptr) = fx_swap (FIXNUM_AR (lptr), AR_SIZE (lptr));
               break;
            case CL_AR_FLOAT:
               FLOAT_AR (lptr) = fl_swap (FLOAT_AR (lptr), AR_SIZE (lptr));
               break;
            case CL_AR_CHAR:
               CHAR_AR (lptr) = ch_swap (CHAR_AR (lptr), AR_SIZE (lptr));
            }
         }
         break;
         
      /*--------------------------------------------------*/
      case CL_IND:
         GET_FORM (form) = form_swap (lptr, 1L);
         LOAD_FORWARD (GET_FORM (form), lptr);
         form = GET_FORM (form);
         goto begin;

      /* Foreign-Datentypen */
      /*--------------------*/
      case CL_C_CHAR:
      case CL_C_UNSIGNED_CHAR:
      case CL_C_SHORT:
      case CL_C_INT:
      case CL_C_LONG:
      case CL_C_UNSIGNED_SHORT:
      case CL_C_UNSIGNED_INT:
      case CL_C_UNSIGNED_LONG:
         break;

      default:
         fprintf (stderr, ";;; Unkonwn data type %d by GC\n", TYPE_OF (form));
         exit (1);
      }
   }
   } /*switch*/
}
#endif
