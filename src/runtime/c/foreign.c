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
 * Function : Foreign function interface
 *
 * $Revision: 1.23 $
 * $Id: foreign.c,v 1.23 1994/12/17 11:57:20 pm Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>

#if __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include <string.h>
#include "sys.h"

#include <limits.h>
#include <values.h>
#define MIN_CHAR SCHAR_MIN
#define MAX_CHAR SCHAR_MAX
#define MAX_UNSIGNED_CHAR UCHAR_MAX
#define MIN_SHORT SHRT_MIN
#define MAX_SHORT SHRT_MAX
#define MAX_UNSIGNED_SHORT USHRT_MAX
#define MIN_INT INT_MIN
#define MAX_INT INT_MAX
#define MAX_UNSIGNED_INT UINT_MAX
#define MIN_LONG LONG_MIN
#define MAX_LONG LONG_MAX
#define MAX_UNSIGNED_LONG ULONG_MAX
#define MIN_FLOAT -MAXFLOAT
#define MAX_FLOAT MAXFLOAT
#define MIN_DOUBLE -MAXDOUBLE
#define MAX_DOUBLE MAXDOUBLE

/*------------------------------------------------------------------------------
 * globale Variable, in der die aktuelle Stackposition beim Aufruf
 * einer Call-Out-Funktion gemerkt wird, damit beim Aufruf einer
 * Call-In-Funktion diese Stelle verwendet werden kann.
 *----------------------------------------------------------------------------*/
CL_FORM *save_stack;

/*------------------------------------------------------------------------------
 * wird im erzeugten Code verwendet.
 *----------------------------------------------------------------------------*/
char IllegalType[] = "illegal Type in Call-Out";


/*******************************************************************************
 * Konvertierungsfunktionen von Lisp nach C und von C nach C
 * elementare C-Typen
 * Lisp-Typen,
 * Zeichenketten
 *
 * Argument: object
 ******************************************************************************/

/*--- c-char -----------------------------------------------------------------*/
/* Aufruf, wenn object vom Typ Charakter */
LISP_FUN(rt_make_c_char) { MAKE_CL_C_CHAR(ARG(1), ARG(0)); }

/* Aufruf, wenn object genuegend kleine Zahl */
LISP_FUN(rt_make_c_char_2) { MAKE_CL_C_CHAR_2(ARG(1), ARG(0)); }

/* Aufruf, wenn object C-Typ */
LISP_FUN(rt_cast_c_char) { CAST_CL_C_CHAR(ARG(1), ARG(0)); }

/*--- c-short ----------------------------------------------------------------*/
LISP_FUN(rt_make_c_short) { MAKE_CL_C_SHORT(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_short) { CAST_CL_C_SHORT(ARG(1), ARG(0)); }

/*--- c-int ------------------------------------------------------------------*/
LISP_FUN(rt_make_c_int) { MAKE_CL_C_INT(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_int) { CAST_CL_C_INT(ARG(1), ARG(0)); }

/*--- c-long -----------------------------------------------------------------*/
LISP_FUN(rt_make_c_long) { MAKE_CL_C_LONG(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_long) { CAST_CL_C_LONG(ARG(1), ARG(0)); }

/*--- c-unsigned-char --------------------------------------------------------*/
LISP_FUN(rt_make_c_unsigned_char) { MAKE_CL_C_UNSIGNED_CHAR(ARG(1), ARG(0)); }
LISP_FUN(rt_make_c_unsigned_char_2) { MAKE_CL_C_UNSIGNED_CHAR_2(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_unsigned_char) { CAST_CL_C_UNSIGNED_CHAR(ARG(1), ARG(0)); }

/*--- c-unsigned-short -------------------------------------------------------*/
LISP_FUN(rt_make_c_unsigned_short) { MAKE_CL_C_UNSIGNED_SHORT(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_unsigned_short) { CAST_CL_C_UNSIGNED_SHORT(ARG(1), ARG(0)); }

/*--- c-unsigned-int ---------------------------------------------------------*/
LISP_FUN(rt_make_c_unsigned_int) { MAKE_CL_C_UNSIGNED_INT(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_unsigned_int) { CAST_CL_C_UNSIGNED_INT(ARG(1), ARG(0)); }

/*--- c-unsigned-long --------------------------------------------------------*/
LISP_FUN(rt_make_c_unsigned_long) { MAKE_CL_C_UNSIGNED_LONG(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_unsigned_long) { CAST_CL_C_UNSIGNED_LONG(ARG(1), ARG(0)); }

/*--- c-float ----------------------------------------------------------------*/
LISP_FUN(rt_make_c_float) { MAKE_CL_C_FLOAT(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_float) { CAST_CL_C_FLOAT(ARG(1), ARG(0)); }

/*--- c-double ---------------------------------------------------------------*/
LISP_FUN(rt_make_c_double) { MAKE_CL_C_DOUBLE(ARG(1), ARG(0)); }
LISP_FUN(rt_cast_c_double) { CAST_CL_C_DOUBLE(ARG(1), ARG(0)); }

/*--- c-string ---------------------------------------------------------------*/
LISP_FUN(rt_make_c_string)
{
   char *loc = get_c_string(ARG(0));
   size_t length = strlen(loc) + 1;        /* Laenge aus Slot auslesen !?!?!? */
   char *addr = (char *)malloc(length);

   (void)strcpy(addr, loc);
   MAKE_CL_C_STRING(ARG(1), ARG(0), addr);
}

LISP_FUN(rt_copy_c_string)
{
   char *loc = GET_C_STRING(ARG(0));
   size_t length = strlen(loc) + 1;
   char *addr = (char *)malloc(length);
   
   (void)strcpy(addr, loc);
   MAKE_CL_C_STRING(ARG(1), ARG(0), addr);
}


/*------------------------------------------------------------------------------
 * Typpraedikate
 *----------------------------------------------------------------------------*/
LISP_FUN(c_primitive_p) 
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)));
}

LISP_FUN(c_floating_p) 
{
   RET_BOOL_OPT(CL_C_FLOATING_P(ARG(0)));
}


LISP_FUN(c_char_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_CHARACTER(ARG(0)) >= MIN_CHAR) &&
                (GET_C_CHARACTER(ARG(0)) <= MAX_CHAR));
}

LISP_FUN(c_short_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_INTEGER(ARG(0)) >= MIN_SHORT) &&
                (GET_C_INTEGER(ARG(0)) <= MAX_SHORT));
}

LISP_FUN(c_int_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_INTEGER(ARG(0)) >= MIN_INT) &&
                (GET_C_INTEGER(ARG(0)) <= MAX_INT));
}

LISP_FUN(c_long_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_INTEGER(ARG(0)) >= MIN_LONG) &&
                (GET_C_INTEGER(ARG(0)) <= MAX_LONG));
}

LISP_FUN(c_unsigned_char_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_CHARACTER(ARG(0)) >= 0) &&
                (GET_C_CHARACTER(ARG(0)) <= MAX_UNSIGNED_CHAR));
}

LISP_FUN(c_unsigned_short_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_INTEGER(ARG(0)) >= 0) &&
                (GET_C_INTEGER(ARG(0)) <= MAX_UNSIGNED_SHORT));
}

LISP_FUN(c_unsigned_int_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_INTEGER(ARG(0)) >= 0) &&
                (GET_C_INTEGER(ARG(0)) <= MAX_UNSIGNED_INT));
}

LISP_FUN(c_unsigned_long_p)
{
   RET_BOOL_OPT(CL_C_PRIMITIVE_P(ARG(0)) &&
                (GET_C_INTEGER(ARG(0)) >= 0) &&
                (GET_C_INTEGER(ARG(0)) <= MAX_UNSIGNED_LONG));
}

LISP_FUN(c_float_p)
{
   RET_BOOL_OPT(CL_C_FLOATING_P(ARG(0)) &&
                (GET_C_FLOATING(ARG(0)) >= MIN_FLOAT) &&
                (GET_C_FLOATING(ARG(0)) <= MAX_FLOAT));
}

LISP_FUN(c_double_p)
{
   RET_BOOL_OPT(CL_C_FLOATING_P(ARG(0)) &&
                (GET_C_FLOATING(ARG(0)) >= MIN_DOUBLE) &&
                (GET_C_FLOATING(ARG(0)) <= MAX_DOUBLE));
}


/*******************************************************************************
 * Konvertierungsfunktionen von C nach Lisp
 * elementare C-Typen
 * Zeichenketten
 *
 * Argument: object
 ******************************************************************************/

LISP_FUN(rt_make_lisp_character) { MAKE_LISP_CHARACTER(ARG(1), ARG(0)); }
LISP_FUN(rt_make_lisp_integer)   { MAKE_LISP_INTEGER(ARG(1), ARG(0)); }
LISP_FUN(rt_make_lisp_float)     { MAKE_LISP_FLOAT(ARG(1), ARG(0)); }

LISP_FUN(rt_make_lisp_string)
{
   char *loc = GET_C_STRING(ARG(0));

   make_string(ARG(0), loc);
}


/*------------------------------------------------------------------------------
 * Laufzeitsystemfunktionen fuer die restlichen C-Typen.
 * Strukturen
 * Varianten
 * Felder
 * Handles
 *----------------------------------------------------------------------------*/
/*--- Kopier-Funktionen ---*/
/* Drei Argumente: neue Struktur, alte Struktur, Groesse */
LISP_FUN(rt_copy_c_struct)
{
   memcpy(GET_C_STRUCT_PTR(ARG(0)), 
          GET_C_STRUCT_PTR(ARG(1)),
          GET_C_UNSIGNED_LONG(ARG(2)));
}

LISP_FUN(rt_copy_c_union)
{
   memcpy(GET_C_UNION_PTR(ARG(0)), 
          GET_C_UNION_PTR(ARG(1)),
          GET_C_UNSIGNED_LONG(ARG(2)));
}

LISP_FUN(rt_copy_c_array)
{
   memcpy(GET_C_ARRAY_PTR(ARG(0)), 
          GET_C_ARRAY_PTR(ARG(1)),
          GET_C_UNSIGNED_LONG(ARG(2)));
}

/*--- Typpraedikate ---*/
/* Zwei Argumente: Struktur, Typpraedikat */
LISP_FUN(rt_struct_p)
{
   RET_BOOL_OPT(CL_C_STRUCT_P(ARG(0)) && 
                (GET_C_TYPESYMBOL(ARG(0)) == GET_SYMBOL(ARG(1))));
}

LISP_FUN(rt_union_p)
{
   RET_BOOL_OPT(CL_C_UNION_P(ARG(0)) && 
                (GET_C_TYPESYMBOL(ARG(0)) == GET_SYMBOL(ARG(1))));
}

LISP_FUN(rt_array_p)
{
   RET_BOOL_OPT(CL_C_ARRAY_P(ARG(0)) && 
                (GET_C_TYPESYMBOL(ARG(0)) == GET_SYMBOL(ARG(1))));
}

LISP_FUN(rt_handle_p)
{
   RET_BOOL_OPT(CL_C_HANDLE_P(ARG(0)) && 
                (GET_C_TYPESYMBOL(ARG(0)) == GET_SYMBOL(ARG(1))));
}


/*------------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/
LISP_FUN(FFI_free)
{
   int type = F_TYPE_OF(ARG(0));
   
   switch (type) 
   {
   case CL_C_STRUCT:
      free((void *)GET_C_STRUCT_PTR(ARG(0)));
      break;
   case CL_C_UNION:
      free((void *)GET_C_UNION_PTR(ARG(0)));
      break;
   case CL_C_ARRAY:
      free((void *)GET_C_ARRAY_PTR(ARG(0)));
      break;
   case CL_C_STRING:
      free((void *)GET_C_STRING(ARG(0)));
      break;
   }
   LOAD_NIL(ARG(0));
}



/*******************************************************************************
 * Fuer die Ausgabe eine Einfache Form
 * Argument: object
 ******************************************************************************/
void c_struct_p (base)
CL_FORM *base;
{
   RET_BOOL_OPT(CL_C_STRUCT_P(ARG(0)));
}

void c_union_p (base)
CL_FORM *base;
{
   RET_BOOL_OPT(CL_C_UNION_P(ARG(0)));
}

void c_array_p (base)
CL_FORM *base;
{
   RET_BOOL_OPT(CL_C_ARRAY_P(ARG(0)));
}

void c_handle_p (base)
CL_FORM *base;
{
   RET_BOOL_OPT(CL_C_HANDLE_P(ARG(0)));
}


void rt_internal_get_symbol (base)
CL_FORM *base;
{
   LOAD_SYMBOL(GET_SYMBOL(OFFSET(GET_FORM(ARG(0)), 0)), ARG(0));
}

void rt_internal_get_address (base)
CL_FORM *base;
{
   LOAD_FIXNUM(ARG(1), (long)GET_CHAR_PTR(OFFSET(GET_FORM(ARG(0)), 2)), ARG(0));
}


/*------------------------------------------------------------------------------
 * Funktionen zum Bestimmen der Werte-Bereiche der C-Datentypen
 *----------------------------------------------------------------------------*/

LISP_FUN(rt_check_char)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= MIN_CHAR) && 
            (GET_FIXNUM(ARG(0)) <= MAX_CHAR));
}

LISP_FUN(rt_check_short)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= MIN_SHORT) && 
            (GET_FIXNUM(ARG(0)) <= MAX_SHORT));
}

LISP_FUN(rt_check_int)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= MIN_INT) && 
            (GET_FIXNUM(ARG(0)) <= MAX_INT));
}

LISP_FUN(rt_check_long)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= MIN_LONG) && 
            (GET_FIXNUM(ARG(0)) <= MAX_LONG));
}

LISP_FUN(rt_check_unsigned_char)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= 0) && 
            (GET_FIXNUM(ARG(0)) <= MAX_UNSIGNED_CHAR));
}

LISP_FUN(rt_check_unsigned_short)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= 0) && 
            (GET_FIXNUM(ARG(0)) <= MAX_UNSIGNED_SHORT));
}

LISP_FUN(rt_check_unsigned_int)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= 0) && 
            (GET_FIXNUM(ARG(0)) <= MAX_UNSIGNED_INT));
}

LISP_FUN(rt_check_unsigned_long)
{
   RET_BOOL(CL_FIXNUMP(ARG(0)) && 
            (GET_FIXNUM(ARG(0)) >= 0) && 
            (GET_FIXNUM(ARG(0)) <= MAX_UNSIGNED_LONG));
}

LISP_FUN(rt_check_float)
{
   RET_BOOL(CL_FLOATP(ARG(0)) &&
            (GET_FLOAT(ARG(0)) >= MIN_FLOAT) &&
            (GET_FLOAT(ARG(0)) <= MAX_FLOAT));
}

LISP_FUN(rt_check_double)
{
   RET_BOOL(CL_FLOATP(ARG(0)) &&
            (GET_FLOAT(ARG(0)) >= MIN_DOUBLE) &&
            (GET_FLOAT(ARG(0)) <= MAX_DOUBLE));
}


