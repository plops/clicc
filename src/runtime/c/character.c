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
 * Funktion : Characters
 *
 * $Revision: 1.20 $
 * $Id: character.c,v 1.20 1995/04/26 13:22:02 uho Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/*------------------------------------------------------------------------------
 * Zeichenklassen-Tests
 *----------------------------------------------------------------------------*/

LISP_FUN(rt_standard_char_p)
{
   int ch = GET_CHAR(ARG(0));
   RET_BOOL_OPT(isprint(ch) || ch == '\n');
}

LISP_FUN(rt_graphic_char_p)
{
   RET_BOOL_OPT(isprint(GET_CHAR(ARG(0))));
}

LISP_FUN(rt_alpha_char_p)
{
   RET_BOOL_OPT(isalpha(GET_CHAR(ARG(0))));
}
   
LISP_FUN(rt_upper_case_p)
{
   RET_BOOL_OPT(isupper(GET_CHAR(ARG(0))));
}

LISP_FUN(rt_lower_case_p)
{
   RET_BOOL_OPT(islower(GET_CHAR(ARG(0))));
}

LISP_FUN(rt_both_case_p)
{
   int ch = GET_CHAR(ARG(0));
   /* Buchstabe, für den ein äquivalenter GROSS/kleinbuchstabe existiert */
   RET_BOOL_OPT(isalnum(ch) && (islower(ch) && toupper(ch) != ch
                                || isupper(ch) && tolower(ch) != ch));
}

/*------------------------------------------------------------------------------
 * RT:DIGIT-CHAR char
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_digit_char)
{
   char buf[2];

   buf[0] = GET_CHAR(ARG(0));
   buf[1] = '\0';

   if (isalnum(buf[0]))
      LOAD_SMALLFIXNUM(strtol(buf, NULL, 36), ARG(0));
   else
      LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::CHAR-CODE char
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_char_code)
{
    LOAD_SMALLFIXNUM((int)GET_CHAR(ARG(0)), ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::CODE-CHAR code
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_code_char)
{
    LOAD_CHAR(ARG(1), GET_FIXNUM(ARG(0)), ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::CHAR-UPCASE charcode
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_char_upcase)
{
   int ch = GET_CHAR(ARG(0));
   LOAD_CHAR(ARG(0), islower(ch) ? toupper(ch) : ch, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::CHAR-DOWNCASE charcode
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_char_downcase)
{
   int ch = GET_CHAR(ARG(0));
   LOAD_CHAR(ARG(0), isupper(ch) ? tolower(ch) : ch, ARG(0));
}
   

