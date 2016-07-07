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
 * Function : Objectsystem
 *
 * $Revision: 1.17 $
 * $Id: clos.c,v 1.17 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * lokale Konstante
 *----------------------------------------------------------------------------*/
#define INSTANCE_HEADER 1


/*------------------------------------------------------------------------------
 * RT::MAKE-INSTANCE size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_make_instance)
{
   long size = GET_FIXNUM(ARG(0));
   CL_FORM *vector;

   vector = form_alloc(ARG(0), size + INSTANCE_HEADER);
   INIT_INSTANCE(vector, size);
   LOAD_INSTANCE(vector, ARG(0));
}

