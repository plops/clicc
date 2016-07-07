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
 * Function : Lists
 *
 * $Revision: 1.12 $
 * $Id: list.c,v 1.12 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/* Laufzeitfehlermeldungen */

char No_list[] = "~a is not a list";

/*------------------------------------------------------------------------------
 * LIST &rest args                                                              
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Flist)
{
   if(nargs == 0)               /* (list) = () */
   {
      LOAD_NIL(ARG(0));
   }
   else
   {
      CL_FORM *lptr, *lp;
      int i;

      lptr = form_alloc(ARG(nargs), nargs * CONS_SIZE);
      nargs--;
      for(lp = lptr, i = 0; i < nargs; lp += CONS_SIZE, i++)
      {
         INIT_CONS(lp);
         COPY(ARG(i),CAR(lp));
         LOAD_CONS(lp + CONS_SIZE, CDR(lp));
      }
      INIT_CONS(lp);
      COPY(ARG(i), CAR(lp));
      LOAD_NIL(CDR(lp));
      LOAD_CONS(lptr, ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * LIST* arg &rest others                                                       
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(FlistX)
{
   CL_FORM *lptr, *lp;
   int i;

   if(nargs == 1)               /* (list* x) = x */
      return;

   nargs--;
   lptr = form_alloc(ARG(nargs + 1),  nargs * CONS_SIZE);
   for(lp = lptr, i = 0; i < nargs; lp += CONS_SIZE, i++)
   {
      INIT_CONS(lp);
      COPY(ARG(i), CAR(lp));
      LOAD_CONS(lp + CONS_SIZE, CDR(lp));
   }
   COPY(ARG(i), CDR(lp - CONS_SIZE));
   LOAD_CONS(lptr, ARG(0));
}

/*------------------------------------------------------------------------------
 * APPEND &rest lists                                                           
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fappend)
{
   int i, list_len = 0;
   CL_FORM *lptr, *lp1, *lp2;

   switch(nargs)
   {
   case 0:                      /* (APPEND) = NIL */
      LOAD_NIL(ARG(0));
      return;
   case 1:                      /* (APPEND arg) = arg */
      return;
   default:
      /* Gesamtlistenlänge der ersten  N - 1 Argumente bestimmen */
      /*---------------------------------------------------------*/
      nargs--;
      for(i = 0; i < nargs; i++) 
      {
         if (CL_CONSP(ARG(i)))
            for (lptr = ARG(i); CL_CONSP(lptr); lptr = GET_CDR(lptr))
               list_len++;
         else if (!CL_NILP(ARG(i)))
            Lerror(ARG(i), No_list);
      }
      
      /* Liste leer, letztes Argument zurückgeben */
      /*------------------------------------------*/
      if(list_len == 0)
      {
         COPY(ARG(nargs), ARG(0));
         return;
      }

      /* Kopieren der ersten N - 1 Listen */
      /*----------------------------------*/
      lptr = form_alloc(ARG(nargs + 1), list_len * CONS_SIZE);

      for (lp1 = lptr, i = 0; i < nargs; i++)
      {
         if (CL_CONSP(ARG(i))) /* nur CONS oder NIL möglich */
         {
            for (lp2 = ARG(i); CL_CONSP(lp2); lp2 = GET_CDR(lp2))
            {
               INIT_CONS(lp1);
               COPY(GET_CAR(lp2), CAR(lp1));
               LOAD_CONS(lp1 + CONS_SIZE, CDR(lp1));
               lp1 += CONS_SIZE;
            }
         }
      }
      /* Anhängen der Restliste */
      /*------------------------*/
      COPY(ARG(i), CDR(lp1 - CONS_SIZE));
      LOAD_CONS(lptr, ARG(0));
   }
}
