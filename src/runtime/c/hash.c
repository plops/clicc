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
 * Function : Low Level Hash-Functions
 *
 * $Revision: 1.14 $
 * $Id: hash.c,v 1.14 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * Berechnet einen Hash-Index (unsigned long) aus den Zeichen eines C-Strings.
 *----------------------------------------------------------------------------*/
unsigned long hash(str)
char *str;
{
   unsigned char c;
   unsigned long ret = 0;
   
   while ((c = *str++) != 0)
   {
      ret <<= 4;
      ret += c;
   }
   return(ret);
}

/*------------------------------------------------------------------------------
 * RT::SXHASH-STRING string
 * Berechnet einen Hash-Index (Fixnum) aus einem simple-string
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_sxhash_string)
{
   LOAD_FIXNUM(ARG(0), hash(get_c_string(ARG(0))), ARG(0));
}
