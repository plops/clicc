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
 * Content  : Arithmetic Operations
 *
 * $Revision: 1.2 $
 * $Id: arith.c,v 1.2 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * Liefert ein LISP-Argument als C-FLOAT
 *----------------------------------------------------------------------------*/
double get_float(base)
CL_FORM *base;
{
   if (CL_FIXNUMP(ARG(0)))
   	return((double)GET_FIXNUM(ARG(0)));
   else if (CL_FLOATP(ARG(0)))
   	return(GET_FLOAT(ARG(0)));
   else
      no_num_err(ARG(0));
   return 0; /* damit lint zufrieden ist */
}

/*------------------------------------------------------------------------------
 * Alloziert die Floating-point Zahl 'num_float' im Heap.
 * 'base' zeigt auf den ersten freien Eintrag im Stack (GC).
 * Rueckgabewert: Zeiger auf die neu allozierte Zahl
 *----------------------------------------------------------------------------*/
double *make_float(base, num_float)
CL_FORM *base;
double   num_float;
{
   double *heap_float = float_alloc(base, 1L);
   *heap_float = num_float;
   return heap_float;
}

/*------------------------------------------------------------------------------
 * Print error message in `number' functions
 *----------------------------------------------------------------------------*/
LISP_FUN(no_num_err)
{
   Lerror(ARG(0), "~a is not a number");
}

/*------------------------------------------------------------------------------
 * + &rest numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fplus)
{
   int i;
   long   num_fixnum = 0;       /* LISP OBJECT of type FIXNUM */
   double num_float  = 0;       /* LISP OBJECT of type FLOAT  */
   BOOL   float_res  = FALSE;   /* Kommt in 'numbers' eine FLOAT vor ? */
 
   for(i = 0; i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	num_fixnum += GET_FIXNUM(ARG(i));
         break;
      case CL_FLOAT:
      	num_float += GET_FLOAT(ARG(i));
      	float_res = TRUE;
         break;
      default: no_num_err(ARG(i));
      }
   }
   if(float_res)
	{
      num_float += num_fixnum;
      LOAD_FLOAT(ARG(0), make_float(ARG(0), num_float), ARG(0));
	}
   else
      LOAD_FIXNUM(ARG(0), num_fixnum, ARG(0));
}

/*------------------------------------------------------------------------------
 * - &rest numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fminus)
{
   int i;
   long   num_fixnum = 0;       /* LISP OBJECT of type FIXNUM */
   double num_float  = 0;       /* LISP OBJECT of type FLOAT  */
   BOOL   float_res = FALSE;    /* Ist das Ergebnis eine FLOAT ? */
 
   if(nargs > 1)
	{
      switch(TYPE_OF(ARG(0)))
      {
      case CL_FIXNUM:
      	num_fixnum = GET_FIXNUM(ARG(0));
         break;
      case CL_FLOAT:
      	num_float = GET_FLOAT(ARG(0));
      	float_res = TRUE;
         break;
      default: no_num_err(ARG(0));
      }
   }
   for(i =(nargs > 1 ? 1 : 0); i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	num_fixnum -= GET_FIXNUM(ARG(i));
         break;
      case CL_FLOAT:
      	num_float -= GET_FLOAT(ARG(i));
      	float_res = TRUE;
         break;
      default: no_num_err(ARG(i));
      }
   }
   if(float_res)
	{
      num_float += num_fixnum;
      LOAD_FLOAT(ARG(0), make_float(ARG(0), num_float), ARG(0));
	}
   else
      LOAD_FIXNUM(ARG(0), num_fixnum, ARG(0));
}

/*------------------------------------------------------------------------------
 * * &rest numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fmult)
{
   int i;
   long   num_fixnum = 1;       /* LISP OBJECT of type FIXNUM */
   double num_float  = 1;       /* LISP OBJECT of type FLOAT  */
   BOOL   float_res = FALSE;    /* Ist das Ergebnis vom Typ FLOAT ? */
 
   for(i = 0; i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	num_fixnum *= GET_FIXNUM(ARG(i));
         break;
      case CL_FLOAT:
      	num_float *= GET_FLOAT(ARG(i));
      	float_res = TRUE;
         break;
      default: no_num_err(ARG(i));
      }
   }
   if(float_res)
	{
      num_float *= num_fixnum;
      LOAD_FLOAT(ARG(0), make_float(ARG(0), num_float), ARG(0));
	}
   else
      LOAD_FIXNUM(ARG(0), num_fixnum, ARG(0));
}

/*------------------------------------------------------------------------------
 * / number &rest more-numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fdiv)
{
   int i;
   long   num_fixnum = 1;       /* LISP OBJECT of type FIXNUM */
   long   div_fixnum;
   double num_float  = 1;       /* LISP OBJECT of type FLOAT  */
   double div_float;
   BOOL   float_res = FALSE;    /* Ist das Ergebnis eine FLOAT ? */
 
   if(nargs > 1)
	{
      switch(TYPE_OF(ARG(0)))
      {
      case CL_FIXNUM:
      	num_fixnum = GET_FIXNUM(ARG(0));
         break;
      case CL_FLOAT:
      	num_float = GET_FLOAT(ARG(0));
      	float_res = TRUE;
         break;
      default: no_num_err(ARG(0));
      }
   }
   for(i =(nargs > 1 ? 1 : 0); i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	div_fixnum = GET_FIXNUM(ARG(i));
      	if(div_fixnum == 0)
      		goto div_by_zero;
      	if(! float_res)
         {
      		if(num_fixnum % div_fixnum == 0)
      			num_fixnum /= div_fixnum;
      		else
            {
      			num_float =(double)num_fixnum / div_fixnum;
      			float_res = TRUE;
            }
         }
      	else
         {
      		num_float /= div_fixnum;
         }
         break;
      case CL_FLOAT:
      	div_float = GET_FLOAT(ARG(i));
      	if(div_float == 0)
      		goto div_by_zero;
      	if(float_res)
      		num_float /= div_float;
      	else
         {
      		num_float = num_fixnum / div_float;
      		float_res = TRUE;
         }
         break;
      default: no_num_err(ARG(i));
      }
   }
   if(float_res)
	{
      LOAD_FLOAT(ARG(0), make_float(ARG(0), num_float), ARG(0));
	}
   else
      LOAD_FIXNUM(ARG(0), num_fixnum, ARG(0));
   return;

 div_by_zero:
	Labort("Division by zero");
}

/*------------------------------------------------------------------------------
 * 1+ number
 *----------------------------------------------------------------------------*/
LISP_FUN(F1plus)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
      /* FIXNUM inkrementieren */
   	LOAD_FIXNUM(ARG(1), GET_FIXNUM(ARG(0)) + 1, STACK(base,0)); 
      break;                    /* wird als Resultat zur"uckgeliefert */
   case CL_FLOAT:
      /* Eine neue Fliesskommazahl im Heap allozieren und 'number' + 1 dort
         abspeichern */
		LOAD_FLOAT(ARG(1), make_float(ARG(0), GET_FLOAT(ARG(0)) + 1),
                 ARG(0));
      break;
   default: no_num_err(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * 1- number
 *----------------------------------------------------------------------------*/
LISP_FUN(F1minus)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
      /* FIXNUM dekrementieren */
   	LOAD_FIXNUM(ARG(1), GET_FIXNUM(ARG(0)) - 1, ARG(0)); 
      break;                    /* wird als Resultat zur"uckgeliefert */
   case CL_FLOAT:
      /* Eine neue Fliesskommazahl im Heap allozieren und 'number' - 1 dort
         abspeichern */
		LOAD_FLOAT(ARG(1), make_float(ARG(0), GET_FLOAT(ARG(0)) - 1),
                 ARG(0));
      break;
   default: no_num_err(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * 1-fix fixnum
 *----------------------------------------------------------------------------*/
LISP_FUN(F1minusfix)
{
   /* FIXNUM dekrementieren */
 	LOAD_FIXNUM(ARG(1), GET_FIXNUM(ARG(0)) - 1, ARG(0)); 
}
