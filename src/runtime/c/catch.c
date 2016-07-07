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
 *  Funktion : - Restaurieren von Special-Variablen
 *             - Special-Forms UNWIND-PROTECT, CATCH, THROW
 *               implementiert als Systemfunktionen.
 *
 * $Revision: 1.12 $
 * $Id: catch.c,v 1.12 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * Fehlermeldung
 *----------------------------------------------------------------------------*/
char NO_CONT[] = "Continuation called outside of its dynamic extent";
char NO_CATCH[] = "Called non existing catch";

/*------------------------------------------------------------------------------
 * lokale Konstanten
 *----------------------------------------------------------------------------*/
#define BINDINGS_STACK_SIZE 1000

/*------------------------------------------------------------------------------
 * globale Variablen
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Binding-Stack wachst von niedrigen zu hohen Adressen; 'bind_top' zeigt auf 
 * den naechsten freien Eintrag.                   
 *----------------------------------------------------------------------------*/
CL_FORM bind_stack[BINDINGS_STACK_SIZE];
CL_FORM *bind_top = bind_stack;

/*------------------------------------------------------------------------------
 * die zuletzt installierte Continuation
 *----------------------------------------------------------------------------*/
CONTENV *last_cont = NULL;

/*------------------------------------------------------------------------------
 * Wenn Pointer nicht verlustfrei in ein int konvertiert werden k"onnen,
 * dann wird dieser Buffer in SETJMP und LONGJMP verwendet.
 *----------------------------------------------------------------------------*/
#ifndef INT_GE_PTR
char *jmp_value;
#endif

/*------------------------------------------------------------------------------
 * unwind_to
 * Binding-Stack abarbeiten, bis 'bind_top' den Wert 'saved_bind_top' hat.
 * ACHTUNG, hier wird davon ausgegangen, dass 'bind_top' von kleinen
 * zu grossen Adressen waechst.
 *----------------------------------------------------------------------------*/
void unwind_to(saved_bind_top)
CL_FORM *saved_bind_top;
{
   while (bind_top > saved_bind_top)
   {
      RESTORE_SPECIAL;
   }
}

/*------------------------------------------------------------------------------
 *  RT::CATCH tag body (funarg)
 *
 *  CATCH erzeugt evtl. MV
 *----------------------------------------------------------------------------*/
void rt_catch (base)
CL_FORM *base;
{
   CONTENV new_cont;
   CL_FORM *caller_base;
   
   new_cont.bind_top = bind_top;
   new_cont.last = last_cont;
   last_cont = &new_cont;
   if ((caller_base = (CL_FORM *)SETJMP(new_cont.jmp_buf)) == NULL)
   {
      /* Nicht von throw */
      /* Aufruf des Rumpfes */
      Ffuncall(ARG(1), 1);
      COPY(ARG(1), ARG(0));

      /* aktuellen Catcher entfernen */
      last_cont = new_cont.last;
   }
   else
   {
      /* Kontroll Transfer von Throw */
      /* aktuellen Catcher entfernen */
      last_cont = new_cont.last;

      /* Pruefen, ob der 1. Parameter von THROW gleich dem 1. Parameter */
      /* von CATCH ist */
      if (EQ(OFFSET(caller_base, 0), ARG(0)))
      {
         /* Passendes Tag gefunden */
         /* Der 2. Parameter von THROW wird das Resultat von CATCH. */
         /* weiter Werte evtl. in 'mv_buf' */
         Fvalues(OFFSET(caller_base, 1), mv_count);
         COPY(OFFSET(caller_base, 1), ARG(0));
      }
      else
         call_cont(caller_base);
   }
}

/*------------------------------------------------------------------------------
 * LISP Parameter:
 * ARG(0): Tag
 * ARG(1): Resultat (+ evtl. Multiple-Values)
 *----------------------------------------------------------------------------*/
void call_cont (base)
CL_FORM *base;
{
   /* kein normales Error benutzen, weil das 'call_cont' benutzten wuerde */
   if (last_cont == NULL)
      Labort(NO_CONT);
   unwind_to(last_cont->bind_top);
   LONGJMP(last_cont->jmp_buf, base);
}

/*------------------------------------------------------------------------------
 * RT::THROW tag result (evtl. Multiple-Values)
 *----------------------------------------------------------------------------*/
void rt_throw(base, nargs)
CL_FORM *base;
int nargs;
{
   /* kein normales Error benutzen, weil das 'call_cont' benutzten wuerde */
   if (last_cont == NULL)
      Labort(NO_CATCH);
   unwind_to(last_cont->bind_top);
   mv_count = nargs - 1;
   LONGJMP(last_cont->jmp_buf, base);
}

/*------------------------------------------------------------------------------
 * (unwind-protect protected-form {cleanup-form}*)
 *
 * --Pass 1-->
 *
 * (rt::unwind-protect
 *    (function (lambda () protected-form))
 *    (function (lambda () {cleanup-form}*))
 * )
 * LISP Parameter:
 * ARG(0): protected Form (Funarg)
 * ARG(1): cleanup Forms (Funarg)
 *
 * unwind-protect erzeugt soviele MV, wie protected-form
 *----------------------------------------------------------------------------*/
void rt_unwind_protect (base)
CL_FORM *base;
{
   CONTENV new_cont;
   CL_FORM *caller_base;
   int local_mv_count;

   new_cont.bind_top = bind_top;
   new_cont.last = last_cont;
   last_cont = &new_cont;
   if((caller_base = (CL_FORM *)SETJMP(new_cont.jmp_buf)) == NULL)
   {
      /* Nicht von throw */
      /* Aufruf der protected Form; Cleanup-Form nicht ueberschreiben */
      COPY(ARG(0), ARG(2));
      Ffuncall(ARG(2), 1);

      /* aktuellen Catcher entfernen */
      last_cont = new_cont.last;

      local_mv_count = mv_count;
      if(local_mv_count > 1)
      {
         /* Multiple-Values die von Protected-Form erzeugt wurde, retten,
            da 'mv_buf' evtl. von Cleanup-Forms benutzt wird. */
         save_values(ARG(2));
      }

      /* Resultat von Protected-Form wird Resultat von UNWIND-PROTECT */
      COPY(STACK(base,2), ARG(0));
      /* Aufruf der Cleanup-Form OBERHALB des Resultats der Protected-Form */
      Ffuncall(ARG(1), 1);

      if(local_mv_count > 1)
      {
         /* Multiple-Values restaurieren */
         Fvalues_list(ARG(0));
      }
   }
   else
   {
      /* Kontroll Transfer von call_cont */
      /* wie bei CATCH, aber nicht Tag pruefen, sondern Cleanup-Form immer */
      /* ausfuehren und mit 'call_cont' weitermachen */

      /* aktuelle Continuation entfernen */
      last_cont = new_cont.last;

      local_mv_count = mv_count;

      if(local_mv_count > 1)
      {
         /* Die von 'call_cont' erzeugten Multiple-Values retten, */
         /* da mv_buf evtl. von Cleanup-Forms benutzt wird. */
         save_values(OFFSET(caller_base, 1));
      }

      /* Aufruf der Cleanup-Form im Stack oberhalb der beiden Argumente */
      /* von THROW  */
      COPY(ARG(1), OFFSET(caller_base, 2));
      Ffuncall(OFFSET(caller_base, 2), 1);

      if(local_mv_count > 1)
      {
         /* Multiple-Values restaurieren */
         Fvalues_list(OFFSET(caller_base, 1));
      }
      call_cont(caller_base);
   }
}
