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
 * Function : - SYMBOL-VALUE
 *            - BOUNDP
 *            - SET
 *            - MAKUNBOUND
 *            - SYMBOL-PLIST
 *            - set-symbol-plist
 *            - SYMBOL-NAME
 *            - MAKE-SYMBOL
 *            - SYMBOL-PACKAGE
 *            - set-symbol-package
 *            - set-constant-flag
 *            - setup-symbols-iterator
 *
 * $Revision: 1.20 $
 * $Id: symbols.c,v 1.20 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/* Referenz in das Lisp-Modul */
/*----------------------------*/
extern void rt_setup_symbol(/* CL_FORM *base */);

/*------------------------------------------------------------------------------
 * RT::MAKE-SYMBOL name
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_make_symbol)
{
   CL_FORM *sym = form_alloc(ARG(1), SYM_SIZE);

   INIT_SYMBOL(sym, ARG(0));  /* benötigt name in ARG(1) */
   LOAD_SYMBOL(sym, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::SETUP-SYMBOLS-ITERATOR first-sym package-vector
 * Wendet setup-symbol auf alle zur Übersetzungszeit definierten
 * Symbole eines Moduls an.
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_setup_symbols_iterator)
{
   CL_FORM *sym = GET_SYMBOL(ARG(0));
   
   /* das Array ist mit einem END_SYMDEF-Eintrag abgeschlossen */
   /* -------------------------------------------------------- */
   while (!IS_END_SYMDEF(sym))
   {
      LOAD_SYMBOL(sym, ARG(2));
      COPY(ARG(1), ARG(3));
      rt_setup_symbol(ARG(2));
      sym += SYM_SIZE; 
   }
}
