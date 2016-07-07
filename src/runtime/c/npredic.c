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
 * Content  : Predicates on Numbers
 *
 * $Revision: 1.3 $
 * $Id: npredic.c,v 1.3 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

char No_integer[]       = "~a is not an integer";

/*------------------------------------------------------------------------------
 * zerop number
 *----------------------------------------------------------------------------*/
LISP_FUN(Fzerop)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	RET_BOOL_OPT(GET_FIXNUM(ARG(0)) == 0);
      break;
   case CL_FLOAT:
   	RET_BOOL_OPT(GET_FLOAT(ARG(0)) == 0.0);
      break;
   default: no_num_err(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * plusp number
 *----------------------------------------------------------------------------*/
LISP_FUN(Fplusp)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	RET_BOOL_OPT(GET_FIXNUM(ARG(0)) > 0);
      break;
   case CL_FLOAT:
   	RET_BOOL_OPT(GET_FLOAT(ARG(0)) > 0.0);
      break;
   default: no_num_err(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * minusp number
 *----------------------------------------------------------------------------*/
LISP_FUN(Fminusp)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	RET_BOOL_OPT(GET_FIXNUM(ARG(0)) < 0);
      break;
   case CL_FLOAT:
   	RET_BOOL_OPT(GET_FLOAT(ARG(0)) < 0.0);
      break;
   default: no_num_err(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * oddp integer
 *----------------------------------------------------------------------------*/
LISP_FUN(Foddp)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	RET_BOOL_OPT(GET_FIXNUM(ARG(0)) & 1);
      break;
   default: Lerror(ARG(0), No_integer);
   }
}

/*------------------------------------------------------------------------------
 * evenp integer
 *----------------------------------------------------------------------------*/
LISP_FUN(Fevenp)
{
   switch(TYPE_OF(ARG(0)))
   {
   case CL_FIXNUM:
   	RET_BOOL_OPT(!(GET_FIXNUM(ARG(0)) & 1));
      break;
   default: Lerror(ARG(0), No_integer);
   }
}
