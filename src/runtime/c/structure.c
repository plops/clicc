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
 * Function : Structures
 *
 * $Revision: 1.14 $
 * $Id: structure.c,v 1.14 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

#define STRUCT_HEADER 2

/*------------------------------------------------------------------------------
 * RT::NEW-STRUCT size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_new_struct)
{
	long size = GET_FIXNUM(ARG(0));
   CL_FORM *structure;
   
   structure = form_alloc(ARG(1), size + STRUCT_HEADER);
   INIT_STRUCT(structure, size);
   LOAD_STRUCT(structure, ARG(0));
}
