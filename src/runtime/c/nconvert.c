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
 * Content  : Type Conversions and Component Extractions on Numbers
 *
 * $Revision: 1.3 $
 * $Id: nconvert.c,v 1.3 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/


#include <math.h>
#include <c_decl.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * RT::FLOAT number
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_float)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   {
   	double num_float = GET_FIXNUM(ARG(0));
   	LOAD_FLOAT(ARG(0), make_float(ARG(0), num_float), ARG(0));
   	break;
   }
   case CL_FLOAT:
   	break;
   default: no_num_err(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * convert-to-int number divisor conversion-type
 *----------------------------------------------------------------------------*/
#define IS_ODD(i) (i & 1)

LISP_FUN(rt_convert_to_int)
{
   int ctype = GET_FIXNUM(ARG(2));

   if(CL_FIXNUMP(ARG(0)) && CL_FIXNUMP(ARG(1)))
   {
      long numerator, denominator, quotient, remainder; 
      BOOL negative = FALSE; 
      
      numerator = GET_FIXNUM(ARG(0));
      if (numerator < 0)
      {
         negative = TRUE;
         numerator = - numerator;
      }
      denominator = GET_FIXNUM(ARG(1));
      if (numerator < 0)
      {
         negative =  NOT(negative); 
         numerator = - numerator;
      }

      /* Divisor und Remainder im positiven Quadranten bestimmen */
      quotient = numerator / denominator;
      remainder = numerator % denominator;

      /* Das truncate von C im Fall von FLOOR, CEILING und ROUND korrigieren */
      /* und das Vorzeicen wieder ber"ucksichtigen */
      switch(ctype)
      {
      case 0:                   /* FLOOR, towards negative infinity */
         if (negative)
         {
            quotient = - quotient;
            if (remainder)
            {
               quotient -= 1;
               remainder = - remainder + denominator;
            }
         }
         break; 

      case 1:                   /* CEILING, towards negative infinity */
         if (negative)
         {
            quotient = - quotient;
            remainder = - remainder;
         }
         else
         {
            
            if (remainder)
            {
               quotient += 1;
               remainder -= denominator;
            }
         }         
         break;
            
      case 2:                   /* TRUNCATE, toward zero */
         if (negative)
         {
            quotient = - quotient;
            remainder = - remainder;
         }
         break;
         
      case 3:                   /* ROUND, to the nearest integer; */
      {                         /* if number is exactly halfway between two */
                                /* integers, then it is rounded to the one */
                                /* that is even*/
         long double_rem = 2 * remainder;

         if (double_rem > denominator ||
             double_rem == denominator && IS_ODD(quotient))
         {
            quotient += 1;
            remainder -= denominator;
         }
         if (negative)
         {
            quotient = - quotient;
            remainder = - remainder;
         }
         break;
      }
      }
      
      LOAD_FIXNUM(ARG(0), quotient, ARG(0));
      LOAD_FIXNUM(ARG(1), remainder, ARG(1)); 
   }
   else
   {
      double quotient, numerator, denominator;
      long   integer;
      switch(TYPE_OF(ARG(0)))
      {
      case CL_FIXNUM:
         numerator = GET_FIXNUM(ARG(0));
         break;
      case CL_FLOAT:
         numerator  = GET_FLOAT(ARG(0));
         break;
      default: no_num_err(ARG(0));
      }
      switch(TYPE_OF(ARG(1)))
      {
      case CL_FIXNUM:
         denominator = GET_FIXNUM(ARG(1));
         break;
      case CL_FLOAT:
         denominator = GET_FLOAT(ARG(1));
         break;
      default: no_num_err(ARG(1));
      }
      quotient = numerator / denominator;

      switch(ctype)
      {
      case 0:                   /* FLOOR */
         integer = floor(quotient);
         break;
      case 1:                   /* CEILING */
         integer = ceil(quotient);
         break;
      case 2:                   /* TRUNCATE */
         integer = quotient;
         break;
      case 3:                   /* ROUND */
         integer = floor(quotient + 0.5);
      
         /* ROUND: x.5 zur geraden Zahl runden */
         if(integer == quotient + 0.5 && IS_ODD(integer))
            integer--;
         break;
      }

      LOAD_FIXNUM(ARG(0), integer, ARG(0));
      LOAD_FLOAT(ARG(1), make_float(ARG(1), numerator - integer * denominator),
                 ARG(1));
   } 
   Fvalues(ARG(0), 2);
}

/*------------------------------------------------------------------------------
 * integer-length integer
 *----------------------------------------------------------------------------*/
LISP_FUN(Finteger_length)
{
   unsigned long i;
   int bits = 0;
   
   if(!CL_FIXNUMP(ARG(0)))
      Lerror(ARG(0), "~a is not a fixnum");

   i = (unsigned long)GET_FIXNUM(ARG(0));
   if((long)i < 0)
      i = ~i;
   
   while(i)
   {
      bits++;
      i >>= 1;
   }
   LOAD_FIXNUM(ARG(0), bits, ARG(0));
}

/*------------------------------------------------------------------------------
 * Die folgenden Funktionen basieren auf der Implementation von floats !!!
 * Augenblicklich erfolgt diese durch C-Daten vom Typ double.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * decode-float float
 * Multiple Werte: 1. Mantisse
 *                 2. Exponent
 *                 3. Vorzeichen ( -1.0 oder 1.0 )
 *----------------------------------------------------------------------------*/
LISP_FUN(Fdecode_float)
{
   double f = GET_FLOAT(ARG(0));
   int n;
   
   LOAD_FLOAT(ARG(0), make_float(base, frexp(f, &n)), ARG(0));
   LOAD_FIXNUM(ARG(0), n, OFFSET(mv_buf, 0));
   LOAD_FLOAT(ARG(0), make_float(base, f < 0.0 ? -1.0 : 1.0), 
              OFFSET(mv_buf, 1));
   mv_count = 3;
}
