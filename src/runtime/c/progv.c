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
 * Funktion : Special Form PROGV implementiert als Systemfunktion
 *
 * $Revision: 1.8 $
 * $Id: progv.c,v 1.8 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

char SYM_EXPECTED[] = "~a is not a symbol";
char TRY_BIND_CONST[] = "can not bind ~a, which is a constant";

/*------------------------------------------------------------------------------
 * RT::PROGV symbol-list value-list body-function
 *----------------------------------------------------------------------------*/
void rt_progv(base)
CL_FORM *base;
{
   CL_FORM *syms = ARG(0), *vals = ARG(1);
   CL_FORM *saved_bind_top = bind_top; 
   
   while(CL_CONSP(syms))
   {
      syms = GET_CAR(syms);
      if(!CL_SYMBOLP(syms))
      {
         COPY(syms, ARG(0)); 
         Lerror(ARG(0), SYM_EXPECTED);
      }
      else if(SYM_IS_CONST(syms))
      {
         COPY(syms, ARG(0)); 
         Lerror(ARG(0), TRY_BIND_CONST);
      }
      else
      {
         /* Symbol + bisherigen Wert auf Binding-Stack retten. */
         /* -------------------------------------------------- */
         SAVE_SPECIAL(GET_FORM(syms));
         if(CL_CONSP(vals))
         {
            /* es sind noch Werte da */
            /* --------------------- */
            vals = GET_CAR(vals);
            COPY(vals, SYM_VALUE(syms));
            vals = CDR(vals);
         }
         else
         {
            /* Liste der Werte ist aufgebraucht */
            /* -------------------------------- */
            LOAD_UNBOUND(SYM_VALUE(syms));
         }
         syms = CDR(syms);
      }
   }

   /* Aufruf des Rumpfes */
   /* ------------------ */
   COPY(ARG(2), ARG(0));
   Ffuncall(ARG(0), 1);

   /* dynamische Variablen restaurieren */
   /* --------------------------------- */
   unwind_to(saved_bind_top);
}



