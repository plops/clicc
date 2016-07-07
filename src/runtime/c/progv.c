/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : Laufzeitsystem
 *            - Special Form PROGV implementiert als Systemfunktion
 *
 * $Revision: 1.5 $
 * $Log: progv.c,v $
 * Revision 1.5  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.4  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.3  1993/02/17  15:42:59  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.2  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

char SYM_EXPECTED[] = "~a is not a symbol";
char TRY_BIND_CONST[] = "can not bind ~a, which is a constant";

/*-------------------------------------------------------------------------*/
/* progv-internal symbol-list value-list body-function                     */
/*-------------------------------------------------------------------------*/
void progv_internal(base)
CL_FORM *base;
{
   CL_FORM *syms = STACK(base, 0), *vals = STACK(base, 1);
   CL_FORM *saved_bind_top = bind_top; 
   
   while(CL_CONSP(syms))
   {
      syms = GET_CAR(syms);
      if(!CL_SYMBOLP(syms))
      {
         COPY(syms, STACK (base, 0)); 
         Lerror(STACK (base, 0), SYM_EXPECTED);
      }
      else if(!CL_NILP(SYM_CONSTFLAG(syms)))
      {
         COPY(syms, STACK (base, 0)); 
         Lerror(STACK (base, 0), TRY_BIND_CONST);
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
            vals++;
         }
         else
         {
            /* Liste der Werte ist aufgebraucht */
            /* -------------------------------- */
            LOAD_UNBOUND(SYM_VALUE(syms));
         }
         syms++;
      }
   }    

   /* Aufruf des Rumpfes */
   /* ------------------ */
   COPY(STACK(base, 2), STACK(base, 0));
   Ffuncall(STACK(base, 0), 1);

   /* dynamische Variablen restaurieren */
   /* --------------------------------- */
   unwind_to(saved_bind_top);
}



