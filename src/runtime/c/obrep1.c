/*------------------------------------------------------------------------------
 * CLiCC: The Common Lisp to C Compiler
 * Copyright (C) 1994 Wolfgang Goerigk, Ulrich Hoffmann, Heinz Knutzen 
 * Christian-Albrechts-Universitaet zu Kiel, Germany
 *------------------------------------------------------------------------------
 * CLiCC has been developed as part of the APPLY research project,
 * funded by the German Ministry of Research and Technology.
 * 
 * CLiCC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CLiCC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License in file COPYING for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *------------------------------------------------------------------------------
 * Funktion : obrep1.c - datenrepräsentationsspezifisch
 *
 * $Revision: 1.16 $
 * $Id: obrep1.c,v 1.16 1995/03/08 15:17:31 wg Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#if OBREP == 1

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

void gc_main();
void save_form();

/*------------------------------------------------------------------------------
 * Speicherbereinigung durchfuehren
 *----------------------------------------------------------------------------*/
void do_gc(top)
CL_FORM *top;
{
   CL_FORM *lptr;

   /* Traversieren der Wurzelbereiche im Hauptmodul und den importierten
    * Modulen
    *--------------------------------------------------------------------*/
   gc_main();

   /* Traversieren des LISP-Laufzeitstacks */
   /* ------------------------------------ */
   for (lptr = stack; lptr < top; lptr++)
      save_form(lptr);
   
   /* Traversieren des Binding-Stacks */
   /* ------------------------------- */
   /* 'bind_top' zeigt auf den naechsten freien Eintrag. */
   for (lptr = bind_stack; lptr < bind_top; lptr++)
      save_form(lptr);
}

/*------------------------------------------------------------------------------
 * Kernroutine des Gabage-Collectors
 * Kopiert alle von "form" erreichbaren Objekte in den neuen Heap.
 *----------------------------------------------------------------------------*/
void save_form (form)
CL_FORM *form;
{
 begin:
   switch (TYPE_OF(form))
   {
      /* keine Garbage-Collection nötig */
      /*--------------------------------*/
   case CL_FIXNUM:
   case CL_CHAR:
   case CL_NIL:
   case CL_DOWNFUN:
   case CL_GLOBFUN:
   case CL_CODE:
   case CL_CFILE:
   case CL_UNIQUE_TAG:
   case CL_C_PRIMITIVE:
   case CL_C_STRING:
      break;
      
      /* Garbage-Collection von Floats */
      /*------------------------------ */
   case CL_FLOAT:
   {
      double *fptr = GET_FLOAT_PTR(form);
      
      if (FL_CONSTANTq(fptr))      /* Konstantes Datum */
         return;
      
      /* Test auf Inkonsistenz im GC */
      
      if (FL_OUT_OF_HEAPq(fptr))
         Labort ("Unexpected pointer out of heap."); 
      if (FL_WRONG_HEAPq(fptr)) 
         Labort ("Unexpected pointer in wrong heap.");
      
      *fl_toh = *fptr;          /* In neuen Heap kopieren */
      GET_FLOAT_PTR(form) = fl_toh++;

      if (fl_toh >= fl_eoh) 
         Labort("Float-Heap Overflow.");
      break;
   }
   default:
   {
      CL_FORM *fptr = GET_FORM(form);
      long size;
      
      /* Annahmen:
       * o Der initialisierte Konstantenbereich wird bereits zur
       *   Übersetzungszeit alloziert.
       * o Nicht initialisierte Arrays werden erst zur Laufzeit alloziert.
       * o Zur Laufzeit allozierte Speicherbereiche liegen adressenmaessig
       *   oberhalb des Programms.
       *--------------------------*/

      if (FO_CONSTANTq(fptr))       /* Konstantes Datum */
         return;                    /* oder #<unbound> Objekt */

      /* Test auf Inkonsistenz im GC */
      if (FO_OUT_OF_HEAPq(fptr)) 
         Labort ("Unexpected pointer out of heap."); 
      if (FO_WRONG_HEAPq(fptr))
         Labort ("Unexpected pointer in wrong heap.");

      /* Wenn Vorwärtsreferenz, dann Datum schon kopiert. */
      if (TYPE_OF(fptr) == GC_FORWARD)
      {
         GET_FORM(form) = GET_FORM(fptr); /* Neue Adresse eintragen */
         return;
      }
      
      switch (TYPE_OF(form))
      {
         /* Garbage-Collection von Listen */
         /*------------------------------ */
      case CL_CONS:
         /* CONS-Knoten kopieren. */
         COPY(fptr,     form_toh);
         COPY(fptr + 1, form_toh + 1);
         
         /* Zeiger auf kopierten CONS-Knoten setzen. */
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         fptr = form_toh;
         form_toh += 2;
         
         /* Depth-First Strategie: Zuerst den CAR weiterverfolgen. */
         save_form(CAR(fptr));

         /* Dann den CDR weiterverfolgen.
          * Durch das goto wird eine unnötige Rekursion vermieden. */
         form = CDR(fptr);
         goto begin;

         /* Garbage-Collection von Symbolen */
         /*---------------------------------*/
      case CL_SYMBOL:
         /* SYMBOL-Daten kopieren. */
         COPY(fptr,     form_toh);
         COPY(fptr + 1, form_toh + 1);
         COPY(fptr + 2, form_toh + 2);
         COPY(fptr + 3, form_toh + 3);
         COPY(fptr + 4, form_toh + 4);
         COPY(fptr + 5, form_toh + 5);

         /* Zeiger auf das neue Symbol */
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         fptr = form_toh;
         form_toh += SYM_SIZE;
         
         /* String kopieren, wenn nicht konstant. */
         if (!AR_CONSTANTq(fptr,OFF_SYM_NAME))
            AR_STRING(fptr + OFF_SYM_NAME) = 
               ch_swap(AR_STRING(fptr + OFF_SYM_NAME), 
                       AR_SIZE(fptr + OFF_SYM_NAME));

         save_form(fptr + OFF_SYM_PLIST);
         save_form(fptr + OFF_SYM_PACKAGE);

         /* Vermeidet Tail-End-Recursion */
         form = fptr + OFF_SYM_VALUE;
         goto begin;

         /* Garbage-Collection von Simple-Vector und ähnlichem */
         /*----------------------------------------------------*/
      case CL_STRUCT:
         size = AR_SIZE(fptr) + 1;
         goto copy;
      case CL_SMVEC_T:
      case CL_INSTANCE:
      case CL_CLOSURE:
         /* Anzahl der zu kopierenden Objekte */
         size = AR_SIZE(fptr);
      copy:
         GET_FORM(form) = form_swap(fptr, size + 1);
         LOAD_FORWARD(GET_FORM(form), fptr);
         fptr = GET_FORM(fptr) + 1L;
         while (--size > 0)
            save_form(fptr++);
         form = fptr;
         goto begin;

      case CL_SMVEC_FIXNUM:
         /* Deskriptor kopieren. */
         COPY(fptr, form_toh);
         COPY(fptr + 1, form_toh + 1);
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         fptr = form_toh;       /* Zeiger auf neuen Header */
         form_toh += 2;
         /* Vektor mit FIXNUM-Repräsentation kopieren. */
         FIXNUM_AR(fptr) = fx_swap(FIXNUM_AR(fptr), AR_SIZE(fptr));
         break;
      case CL_SMVEC_FLOAT:
         /* Deskriptor kopieren. */
         COPY(fptr, form_toh);
         COPY(fptr + 1, form_toh + 1);
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         fptr = form_toh;       /* Zeiger auf neuen Header */
         form_toh += 2;
         /* Vektor mit FLOATs kopieren. */
         FLOAT_AR(fptr) = fl_swap(FLOAT_AR(fptr), AR_SIZE(fptr));
         break;
      case CL_SMVEC_CHARACTER:
         /* Deskriptor kopieren. */
         COPY(fptr, form_toh);
         COPY(fptr + 1, form_toh + 1);
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         fptr = form_toh;       /* Zeiger auf neuen Header */
         form_toh += 2;
         /* String kopieren, wenn nicht konstant. */
         if (!AR_CONSTANTq(fptr,0))
            AR_STRING(fptr) = ch_swap(AR_STRING(fptr), AR_SIZE(fptr));
         break;
      case CL_SMVEC_BIT:
         /* Deskriptor kopieren. */
         COPY(fptr, form_toh);
         COPY(fptr + 1, form_toh + 1);
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         fptr = form_toh;       /* Zeiger auf neuen Header */
         form_toh += 2;
         /* Vektor mit Bitvektor-Repräsentation kopieren. */
         BIT_AR(fptr) = bits_swap(BIT_AR(fptr), AR_SIZE(fptr));
         break;
         
         /* Indirection */
         /*-------------*/
      case CL_IND:
         COPY(fptr, form_toh);
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         form = form_toh++;
         goto begin;

      case CL_C_FLOATING:
      {
         double *fptr = GET_C_FLOATING_PTR(form);
         
         if (FL_CONSTANTq(fptr))      /* Konstantes Datum */
            return;
         
         /* Test auf Inkonsistenz im GC */
         
         if (FL_OUT_OF_HEAPq(fptr))
            Labort ("Unexpected pointer out of heap."); 
         if (FL_WRONG_HEAPq(fptr)) 
            Labort ("Unexpected pointer in wrong heap.");
         
         *fl_toh = *fptr;          /* In neuen Heap kopieren */
         GET_C_FLOATING_PTR(form) = fl_toh++;
         
         if (fl_toh >= fl_eoh) 
            Labort("Float-Heap Overflow.");
         break;
      }
         
      case CL_C_STRUCT:
      case CL_C_UNION:
      case CL_C_ARRAY:
      case CL_C_HANDLE:
         /* CONS-Knoten kopieren. */
         COPY(fptr,     form_toh);
         COPY(fptr + 1, form_toh + 1);
         
         /* Zeiger auf kopierten Knoten setzen. */
         GET_FORM(form) = form_toh;
         LOAD_FORWARD(form_toh, fptr);
         fptr = form_toh;
         form_toh += 2;

         form = fptr;
         goto begin;
         
      default:
         fprintf(stderr, ";;; Unkonwn data type %d by GC\n", TYPE_OF(form));
         exit(1);
      }
   }
   } /*switch*/
}

#endif
