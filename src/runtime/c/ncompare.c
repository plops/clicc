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
 * Content  : Comparisons on Numbers
 *
 * $Revision: 1.2 $
 * $Id: ncompare.c,v 1.2 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>

enum number_type {type_fixnum, type_float};

/*------------------------------------------------------------------------------
 * = number &REST more-numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fnumeql)
{
   int i;

   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   {
   	long first = GET_FIXNUM(ARG(0));

   	for(i = 1; i < nargs; i++)
      {
         switch(TYPE_OF(ARG(i)))
         {
         case CL_FIXNUM:
            if(GET_FIXNUM(ARG(i)) != first)
               goto NIL;
            break;
         case CL_FLOAT:
            if(GET_FLOAT(ARG(i)) != first)
               goto NIL;
            break;
         default: no_num_err(ARG(i));
         }
      }
      goto T;
   }
   case CL_FLOAT:
   {
   	double first = GET_FLOAT(ARG(0));

   	for(i = 1; i < nargs; i++)
      {
         switch(TYPE_OF(ARG(i)))
         {
         case CL_FIXNUM:
            if(GET_FIXNUM(ARG(i)) != first)
               goto NIL;
            break;
         case CL_FLOAT:
            if(GET_FLOAT(ARG(i)) != first)
               goto NIL;
            break;
         default: no_num_err(ARG(i));
         }
      }
      goto T;
   }
   default: no_num_err(ARG(0));
   }
 T:
	LOAD_T(ARG(0));
	return;
 NIL:
	LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * /= number &REST more-numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fnumneql)
{
   int i, j;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */

   for(i = 0; i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      {
      	num_fixnum = GET_FIXNUM(ARG(i));
      	for(j = i + 1; j < nargs; j++)
         {
            switch(TYPE_OF(ARG(j)))
            {
            case CL_FIXNUM:
               if(GET_FIXNUM(ARG(j)) == num_fixnum)
                  goto NIL;
               break;
            case CL_FLOAT:
               if(GET_FLOAT(ARG(j)) == num_fixnum)
                  goto NIL;
               break;
            default: no_num_err(ARG(j));
            }
         }
         break;
      }
      case CL_FLOAT:
      {
      	num_float = GET_FIXNUM(ARG(i));
      	for(j = i + 1; j < nargs; j++)
         {
            switch(TYPE_OF(ARG(j)))
            {
            case CL_FIXNUM:
               if(GET_FIXNUM(ARG(j)) == num_float)
                  goto NIL;
               break;
            case CL_FLOAT:
               if(GET_FLOAT(ARG(j)) == num_float)
                  goto NIL;
               break;
            default: no_num_err(ARG(j));
            }
         }
         break;
      }
      default: no_num_err(ARG(0));
      }
   }
   LOAD_T(ARG(0));
   return;

 NIL:
	LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * < number &REST more-numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Flt)
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(ARG(0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(ARG(0));
   	num_type = type_float;
      break;
   default: no_num_err(ARG(0));
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum < GET_FIXNUM(ARG(i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float < GET_FIXNUM(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(ARG(i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum < GET_FLOAT(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float < GET_FLOAT(ARG(i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(ARG(i));
         break;
      default: no_num_err(ARG(i));
      }
   }
   LOAD_T(ARG(0));
   return;

 NIL:
	LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * <fix fixnum fixnum
 *----------------------------------------------------------------------------*/
LISP_FUN(Fltfix)
{
  if(!(GET_FIXNUM(ARG(0)) < GET_FIXNUM(ARG(1))))
     LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * > number &REST more-numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fgt)
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(ARG(0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(ARG(0));
   	num_type = type_float;
      break;
   default: no_num_err(ARG(0));
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum > GET_FIXNUM(ARG(i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float > GET_FIXNUM(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(ARG(i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum > GET_FLOAT(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float > GET_FLOAT(ARG(i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(ARG(i));
         break;
      default: no_num_err(ARG(i));
      }
   }
   LOAD_T(ARG(0));
   return;

 NIL:
	LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * <= number &REST more-numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fle)
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(ARG(0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(ARG(0));
   	num_type = type_float;
      break;
   default: no_num_err(ARG(0));
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum <= GET_FIXNUM(ARG(i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float <= GET_FIXNUM(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(ARG(i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum <= GET_FLOAT(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float <= GET_FLOAT(ARG(i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(ARG(i));
         break;
      default: no_num_err(ARG(i));
      }
   }
   LOAD_T(ARG(0));
   return;

 NIL:
	LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * >= number &REST more-numbers
 *----------------------------------------------------------------------------*/
LISP_FUN_NARGS(Fge)
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(ARG(0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(ARG(0));
   	num_type = type_float;
      break;
   default: no_num_err(ARG(0));
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(ARG(i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum >= GET_FIXNUM(ARG(i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float >= GET_FIXNUM(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(ARG(i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum >= GET_FLOAT(ARG(i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float >= GET_FLOAT(ARG(i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(ARG(i));
         break;
      default: no_num_err(ARG(i));
      }
   }
   LOAD_T(ARG(0));
   return;

 NIL:
	LOAD_NIL(ARG(0));
}
