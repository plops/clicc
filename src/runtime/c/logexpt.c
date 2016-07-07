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
 * Content  : Exponential and Logarithmic Functions
 *
 * $Revision: 1.2 $
 * $Id: logexpt.c,v 1.2 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <math.h>
#include <c_decl.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * RT::EXPT base-number power-number
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_expt)
{
   static char Illegal_base_num[] =
      "Illegal base-number ~a with non-integer power-number 0";
   double base_num, power;

   switch(TYPE_OF(ARG(1)))
   {
   case CL_FIXNUM:
   {
   	long power_fixnum = GET_FIXNUM(ARG(1));
      /*
        Falls 'power-number' = 0 vom Typ INTEGER,
        ist das Ergebnis 1 vom Typ von 'base-number'.
        */
      if(power_fixnum == 0L)
      {
         switch(TYPE_OF(ARG(0)))
         {
         case CL_FIXNUM:
            LOAD_FIXNUM(ARG(0), 1, ARG(0));
            return;
         case CL_FLOAT:
            LOAD_FLOAT(ARG(0), make_float(ARG(0), 1.0), ARG(0));
            return;
         default: no_num_err(ARG(0));
         }
      }
      /*
        Sind 'base-number' und 'power-number' beide vom Typ RATIONAL
       (hier INTEGER), ist das Ergebnis exakt.
        */
      if(TYPE_OF(ARG(0)) == CL_FIXNUM)
      {
      	long base_fixnum = GET_FIXNUM(ARG(0));
      	long result      = 1;

      	while(power_fixnum-- > 0)
      		result *= base_fixnum;
      	LOAD_FIXNUM(ARG(0), result, ARG(0));
      	return;
      }

      power = power_fixnum;
      break;
   }
   case CL_FLOAT:
      power = GET_FLOAT(ARG(1));
      if(power == 0.0)
      {
         switch(TYPE_OF(ARG(0)))
         {
         case CL_FIXNUM:
            if(GET_FIXNUM(ARG(0)) == 0L)
               Lerror(ARG(0), Illegal_base_num);
            LOAD_FIXNUM(ARG(0), 1L, ARG(0));
            return;
         case CL_FLOAT:
            if(GET_FLOAT(ARG(0)) == 0.0)
               Lerror(ARG(0), Illegal_base_num);
            LOAD_FLOAT(ARG(0), make_float(ARG(0), 1.0), ARG(0));
            return;
         default: no_num_err(ARG(0));
         }
      }
      break;
   default: no_num_err(ARG(1));
   }
   base_num = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), pow(base_num, power)),
              ARG(0));
}
	
/*------------------------------------------------------------------------------
 * RT::LOG number base
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_log)
{
   double number, base_num;

   number   = get_float(ARG(0));
   base_num = get_float(ARG(1));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), log(number) / log(base_num)),
               ARG(0));
}

/*------------------------------------------------------------------------------
 * sqrt number
 *----------------------------------------------------------------------------*/
LISP_FUN(Fsqrt)
{     
   double number = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), sqrt(number)), ARG(0));
}
