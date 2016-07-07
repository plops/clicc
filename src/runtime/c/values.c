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
 * Function : - VALUES
 *            - VALUES-LIST
 *            - save_values
 *
 * $Revision: 1.9 $
 * $Id: values.c,v 1.9 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

char TOO_MANY_VALUES[] = "Too many values";
char LIST_EXPECTED[] = "~A is not a list";

#define MV_LIMIT 20
#define MV_BUFSIZE (MV_LIMIT - 1)

int mv_count;
CL_FORM mv_buf[MV_BUFSIZE];

/*------------------------------------------------------------------------------
 * die Systemfunktion VALUES                                               
 *----------------------------------------------------------------------------*/
void Fvalues(base, nargs)
CL_FORM *base;
int nargs;
{
   if(nargs == 0)
   {
      LOAD_NIL(ARG(0));
   }
   else if(nargs > MV_LIMIT)
      Labort(TOO_MANY_VALUES);
   else
   {
      int i;

      for(i = 1; i < nargs; i++)
      {
         COPY(ARG(i), &mv_buf[i-1]);
      }
   }
   mv_count = nargs;
}

/*------------------------------------------------------------------------------
 * die Systemfunktion VALUES-LIST                                         
 *----------------------------------------------------------------------------*/
void Fvalues_list (base)
CL_FORM *base;
{
   CL_FORM *values = ARG(0);

   mv_count = 0;

   switch(TYPE_OF(values))
   {
   case CL_NIL:
      /* NIL auf dem Stack */
      return;

   case CL_CONS:
      /* Zeiger auf das 1. Element der Liste */
      values = GET_CAR(values);
      /* 1. Element auf den Stack */
      COPY(values, ARG(0));
      values++;
      break;

   default:
      Lerror(ARG(0), LIST_EXPECTED);
      break;
   }

   while(CL_CONSP(values))
   {
      /* mv_count zunaechst als Index in mv_buf benutzen */
      if(mv_count >= MV_BUFSIZE)
         Labort(TOO_MANY_VALUES);
      values = GET_CAR(values);
      COPY(values, &mv_buf[mv_count]);
      mv_count++;
      values++;
   }

   /* den Eintrag auf dem Stack mitzaehlen */
   mv_count++;
}

/*------------------------------------------------------------------------------
 * base zeigt auf einen Wert im LISP-Stack; dieser Wert wird ersetzt durch
 * eine Liste, bestehend aus dem Wert und dem aktuellen Inhalt von mv_buf
 *----------------------------------------------------------------------------*/
void save_values (base)
CL_FORM *base;
{
   int i;

   for(i = 1; i < mv_count; i++)
      COPY(&mv_buf[i-1], ARG(i));
   Flist(ARG(0), mv_count);
   mv_count = 1;
}
