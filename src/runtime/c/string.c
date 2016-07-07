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
 * Function : Strings
 *
 * $Revision: 1.12 $
 * $Id: string.c,v 1.12 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"
#include <string.h>

/*------------------------------------------------------------------------------
 * Erzeugt aus einem C-String einen LISP-String nach 'base'
 *----------------------------------------------------------------------------*/
void make_string(base, string)
CL_FORM *base;
char    *string;
{
   long str_len = strlen(string);
   CL_FORM *header;
   char *chptr;

   chptr = char_alloc(ARG(0), str_len);
   strncpy(chptr, string, str_len);
   header = form_alloc(ARG(0), 2L);
   LOAD_CHAR_PTR(chptr, AR_BASE(header));
   INIT_VEC_CHAR(header, str_len);
   LOAD_VEC_CHAR(header, ARG(0));
}
