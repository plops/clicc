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
 * Funktion : Berechnung der Kenndaten der Zahen-Repraesentation
 *
 * $Revision: 1.8 $
 * $Id: fspecs.c,v 1.8 1995/03/07 15:27:30 uho Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

#undef VALUES_KNOWN

#ifndef CROSSCOMPILE
#ifdef __STDC__
#define VALUES_KNOWN
#endif
#endif

#ifdef VALUES_KNOWN

#include <float.h>

LISP_FUN(rt_calc_radix)
{
   LOAD_FIXNUM(ARG(0), FLT_RADIX, ARG(0));
}

LISP_FUN(rt_calc_mant_dig)
{
   LOAD_FIXNUM(ARG(0), FLT_MANT_DIG, ARG(0));
}

#include <limits.h>

LISP_FUN(rt_most_positive_fixnum)
{
   long i2 = LONG_MAX >> TAG_BITS;
   LOAD_FIXNUM(ARG(0), i2, ARG(0));
}

LISP_FUN(rt_most_negative_fixnum)
{
   long i2 = LONG_MIN >> TAG_BITS;
   LOAD_FIXNUM(ARG(0), i2, ARG(0));
}

#else

LISP_FUN(rt_calc_radix)
{
   double a, b;

   for (a = 1.0; ((a + 1.0) - a) - 1.0 == 0.0; a += a)
      ;
   for (b = 1.0; (a + b) - a == 0.0; b += b)
      ;
   LOAD_FIXNUM(ARG(0), (a + b) - a, ARG(0));
}

LISP_FUN(rt_calc_mant_dig)
{
   double a, b, beta;
   int it;
   
   for (a = 1.0; ((a + 1.0) - a) - 1.0 == 0.0; a += a)
      ;
   for (b = 1.0; (a + b) - a == 0.0; b += b)
      ;
   beta = (a + b) - a;
   for (b = 1.0, it = 0; (((b + 1.0) - b) - 1.0) == 0.0; it++, b *= beta)
      ;
   LOAD_FIXNUM(ARG(0), it, ARG(0));
}

LISP_FUN(rt_most_positive_fixnum)
{
   long i1 = 1, i2 = 0;
   
   while (i1 > i2)
   {
      i2 = i1;
      i1 = i1 * 2 + 1;
   }
   i2 >>= TAG_BITS;
   LOAD_FIXNUM(ARG(0), i2, ARG(0));
}

LISP_FUN(rt_most_negative_fixnum)
{
   long i1 = -1, i2 = 0;

   while (i1 < i2)
   {
      i2 = i1;
      i1 = i1 * 2 - 1;
   }
   i2 >>= TAG_BITS;
   LOAD_FIXNUM(ARG(0), i2, ARG(0));
}

#endif

