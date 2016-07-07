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
 * Content  : Trigonometric and Related Functions
 *
 * $Revision: 1.2 $
 * $Id: trigonom.c,v 1.2 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include <math.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * sin radians
 *----------------------------------------------------------------------------*/
LISP_FUN(Fsin)
{
   double radians = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), sin(radians)), ARG(0));
}

/*------------------------------------------------------------------------------
 * cos radians
 *----------------------------------------------------------------------------*/
LISP_FUN(Fcos)
{
   double radians = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), cos(radians)), ARG(0));
}

/*------------------------------------------------------------------------------
 * tan radians
 *----------------------------------------------------------------------------*/
LISP_FUN(Ftan)
{
   double radians = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), tan(radians)), ARG(0));
}

/*------------------------------------------------------------------------------
 * asin radians
 *----------------------------------------------------------------------------*/
LISP_FUN(Fasin)
{
   double radians = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), asin(radians)), ARG(0));
}

/*------------------------------------------------------------------------------
 * acos radians
 *----------------------------------------------------------------------------*/
LISP_FUN(Facos)
{
   double radians = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), acos(radians)), ARG(0));
}

/*------------------------------------------------------------------------------
 * atan radians
 *----------------------------------------------------------------------------*/
LISP_FUN(Fatan)
{
   double radians = get_float(ARG(0));
   LOAD_FLOAT(ARG(0), make_float(ARG(0), atan(radians)), ARG(0));
}
