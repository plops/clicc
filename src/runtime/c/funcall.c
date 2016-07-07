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
 * Function : FUNCALL, APPLY
 *
 * $Revision: 1.14 $
 * $Id: funcall.c,v 1.14 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

char TOO_FEW_ARGS[] = "too few arguments";

/*----------------------------------------------------------------------------*/
#define CHK_GENAU(parspec) ((parspec) >= 0)
#define CHK_MIN(parspec) ((parspec) < 0)
#define GET_GENAU(parspec) ((parspec))
#define GET_MIN(parspec) (- (parspec) - 1)
#define CREATE_GENAU(count) (count)
#define CREATE_MIN(count) (- (count) - 1)

/*------------------------------------------------------------------------------
 * FUNCALL fn &rest arguments
 *------------------------------------------------------------------------------
 * Moegliche Typen:
 * - CL_CLOSURE: (zeigt auf eine Folge von LISP-Daten)
 *   - Laenge
 *   - Zeiger auf Code
 *   - Parameter-Spezifikation
 *   - .. - Informationen ueber freie Variablen und Funktionen
 *
 * - CL_GLOBFUN: (zeigt auf eine C-Structur in einem globalen C-Array)
 *   - Zeiger auf Code
 *   - Parameter Spezifikation
 *
 * - CL_DOWNFUN: (zeigt auf eine C-Struktur im C-Stack)
 *   - Zeiger auf Code
 *   - Parameter-Spezifikation
 *   - Zeiger auf Display-Array
 * - CL_UNIQUE_TAG: (Zahl, die eine Continuation eindeutig identifiziert)
 *----------------------------------------------------------------------------*/
void Ffuncall(base, nargs)
CL_FORM *base;
int nargs;
{
   int par_spec;

   nargs--;
   switch (TYPE_OF(ARG(0)))
   {
      case CL_GLOBFUN:
         par_spec = GET_GLOBFUN_PARSPEC(GET_GFARG(ARG(0)));
         if (CHK_GENAU(par_spec))
         {
            /* feste Anzahl von Parametern
             * Anzahl der Parameter wird ueberprueft*/
            if (nargs != GET_GENAU(par_spec))
               Labort(ILLEGAL_ARGS);
            /* Aufruf der Funktion, Parameter sind geprueft */
            GET_GLOBFUN_CODE(GET_GFARG(ARG(0))) (ARG(1));
         }
         else
         {
            /* Mindest-Anzahl der Parameter wird ueberprueft */
            if (nargs < GET_MIN(par_spec))
               Labort(TOO_FEW_ARGS);
            /* Aufruf der Funktion, Mindest-Parameter-Zahl ist geprueft */
            GET_GLOBFUN_CODE(GET_GFARG(ARG(0))) (ARG(1), nargs);
         }
         /* das Resultat an die Stelle kopieren, wo es von der aufrufenden
          * Funktion erwartet wird.*/
         COPY(ARG(1), ARG(0));
         break;

      case CL_CLOSURE:
         par_spec = GET_CLOSURE_PARSPEC(ARG(0));
         if (CHK_GENAU(par_spec))
         {
            /* feste Anzahl von Parametern
             * Anzahl der Parameter wird ueberprueft*/
            if (nargs != GET_GENAU(par_spec))
               Labort(ILLEGAL_ARGS);
            /* Aufruf der Funktion, Parameter sind geprueft
             * Zeiger auf die Closure-Datenstruktur als 1. Parameter */
            GET_CLOSURE_CODE(ARG(0)) (ARG(0));
         }
         else
         {
            /* Mindest-Anzahl der Parameter wird ueberprueft
             *-----------------------------------------------*/
            if (nargs < GET_MIN(par_spec))
               Labort(TOO_FEW_ARGS);

            /* Aufruf der Funktion, Mindest-Parameter-Zahl ist geprueft
             * Zeiger auf die Closure-Datenstruktur als 1. Parameter
             *-------------------------------------------------------*/
            GET_CLOSURE_CODE(ARG(0)) (ARG(0), nargs);
         }
         /* Resultat wurde schon an der richtigen Position erzeugt
          *-------------------------------------------------------*/
         break;

      case CL_DOWNFUN:
         par_spec = GET_DOWNFUN_PARSPEC(GET_DFARG(ARG(0)));
         if (CHK_GENAU(par_spec))
         {
            /* feste Anzahl von Parametern
             * Anzahl der Parameter wird ueberprueft
             *---------------------------------------*/
            if (nargs != GET_GENAU(par_spec))
               Labort(ILLEGAL_ARGS);

            /* Aufruf der Funktion, Parameter sind geprueft
             *----------------------------------------------*/
            GET_DOWNFUN_CODE(GET_DFARG(ARG(0)))
               (ARG(1), GET_DOWNFUN_DISPLAY(GET_DFARG(ARG(0))));
         }
         else
         {
            /* Mindest-Anzahl der Parameter wird ueberprueft
             *-----------------------------------------------*/
            if (nargs < GET_MIN(par_spec))
               Labort(TOO_FEW_ARGS);

            /* Aufruf der Funktion, Mindest-Parameter-Zahl ist geprueft
             *----------------------------------------------------------*/
            GET_DOWNFUN_CODE(GET_DFARG(ARG(0)))
               (ARG(1), GET_DOWNFUN_DISPLAY(GET_DFARG(ARG(0))), nargs);

         }
         /* das Resultat an die Stelle kopieren, wo es von der aufrufenden
          * Funktion erwartet wird.
          *-------------------------*/
         COPY(ARG(1), ARG(0));
         break;

      case CL_UNIQUE_TAG:
         if (nargs != 1)
            Labort(ILLEGAL_ARGS);
         call_cont(ARG(0));

      case CL_SYMBOL:
         Lerror(ARG(0), "Symbol ~A is not a function");

      default:
         Lerror(ARG(0), "~A is not a function");
   }
}

/*------------------------------------------------------------------------------
 * APPLY function arg &rest more-args
 *----------------------------------------------------------------------------*/
void Fapply(base, nargs)
CL_FORM *base;
int nargs;
{
   CL_FORM *last = ARG(nargs - 1);
   CL_FORM *list = last;

   nargs--;

   while (CL_CONSP(list))
   {
      list = GET_CAR(list);
      COPY(list, last);
      list++;
      last++;
      nargs++;
   }
   Ffuncall(base, nargs);
}

/*------------------------------------------------------------------------------
 * rest_apply
 *----------------------------------------------------------------------------*/
void rest_apply(base, nargs, len, rest)
CL_FORM *base;
int nargs, len;
CL_FORM *rest;
{
   CL_FORM *last = ARG(nargs);
   while (len-- > 0)
   {
      COPY(rest, last);
      rest++;
      last++;
      nargs++;
   }
   Ffuncall(base, nargs);
}
