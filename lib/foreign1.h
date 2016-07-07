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
 * Funktion : Laufzeitsystem: Makros des FFI
 *
 * $Revision: 1.7 $
 * $Id: foreign1.h,v 1.7 1995/03/08 15:06:15 wg Exp $
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Definitionen der Datentypen von Foreign-Objekten
 *----------------------------------------------------------------------------*/
#define CL_C_PRIMITIVE       (CL_FOREIGN +  0)
#define CL_C_FLOATING        (CL_FOREIGN +  1)
#define CL_C_STRING          (CL_FOREIGN +  2)

#define CL_C_STRUCT          (CL_FOREIGN +  3)
#define CL_C_UNION           (CL_FOREIGN +  4)
#define CL_C_ARRAY           (CL_FOREIGN +  5)
#define CL_C_HANDLE          (CL_FOREIGN +  6)


/*------------------------------------------------------------------------------
 * Makros für den Zugriff auf die Komponenten eines FOREIGN-Objektes
 *----------------------------------------------------------------------------*/
#define GET_C_INTEGER(loc)            ((loc)->val.i)
#define GET_C_CHARACTER(loc)          ((loc)->val.ch)
#define GET_C_FLOATING(loc)           (*((loc)->val.fl))
#define GET_C_FLOATING_PTR(loc)       ((loc)->val.fl)
#define GET_VOID_PTR(loc)             (void *)(GET_CHAR_PTR(loc))

#define GET_C_CHAR(loc)               (char)((loc)->val.ch)
#define GET_C_SHORT(loc)              (short)((loc)->val.i)
#define GET_C_INT(loc)                (int)((loc)->val.i)
#define GET_C_LONG(loc)               (long)((loc)->val.i)
#define GET_C_UNSIGNED_CHAR(loc)      (unsigned char)((loc)->val.ch)
#define GET_C_UNSIGNED_SHORT(loc)     (unsigned short)((loc)->val.i)
#define GET_C_UNSIGNED_INT(loc)       (unsigned int)((loc)->val.i)
#define GET_C_UNSIGNED_LONG(loc)      (unsigned long)((loc)->val.i)
#define GET_C_FLOAT(loc)              (float)(*((loc)->val.fl))
#define GET_C_DOUBLE(loc)             (double)(*((loc)->val.fl))

#define GET_C_STRING(loc)             (char *)((loc)->val.ch_ptr)

#define GET_C_TYPESYMBOL(loc)         GET_SYMBOL(OFFSET(GET_FORM(loc), 0))

#define GET_C_STRUCT_PTR(loc)         GET_VOID_PTR(OFFSET(GET_FORM(loc), 1))
#define GET_C_UNION_PTR(loc)          GET_VOID_PTR(OFFSET(GET_FORM(loc), 1))
#define GET_C_ARRAY_PTR(loc)          GET_VOID_PTR(OFFSET(GET_FORM(loc), 1))
#define GET_C_HANDLE(loc)             GET_VOID_PTR(OFFSET(GET_FORM(loc), 1))

/*------------------------------------------------------------------------------
 * Prädikate zum Testen des Datentyps
 *----------------------------------------------------------------------------*/
#define F_TYPE_OF(loc)             TYPE_OF(loc)

#define CL_C_PRIMITIVE_P(loc)      (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_FLOATING_P(loc)       (F_TYPE_OF(loc) == CL_C_FLOATING)
#define CL_C_STRUCTURED_P(loc)     CL_C_STRUCT_P(loc) || CL_C_UNION_P(loc) || \
                                   CL_C_ARRAY_P(loc) || CL_C_HANDLE_P(loc)

#define CL_C_CHAR_P(loc)           (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_SHORT_P(loc)          (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_INT_P(loc)            (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_LONG_P(loc)           (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_UNSIGNED_CHAR_P(loc)  (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_UNSIGNED_SHORT_P(loc) (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_UNSIGNED_INT_P(loc)   (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_UNSIGNED_LONG_P(loc)  (F_TYPE_OF(loc) == CL_C_PRIMITIVE)
#define CL_C_FLOAT_P(loc)          (F_TYPE_OF(loc) == CL_C_FLOATING)
#define CL_C_DOUBLE_P(loc)         (F_TYPE_OF(loc) == CL_C_FLOATING)
#define CL_C_STRING_P(loc)         (F_TYPE_OF(loc) == CL_C_STRING)

#define CL_C_STRUCT_P(loc)         (F_TYPE_OF(loc) == CL_C_STRUCT)
#define CL_C_UNION_P(loc)          (F_TYPE_OF(loc) == CL_C_UNION)
#define CL_C_ARRAY_P(loc)          (F_TYPE_OF(loc) == CL_C_ARRAY)
#define CL_C_HANDLE_P(loc)         (F_TYPE_OF(loc) == CL_C_HANDLE)

/*------------------------------------------------------------------------------
 * Makros zum Laden der FOREIGN-Objekte
 *----------------------------------------------------------------------------*/
#define LOAD_C_CHAR(top, chr, loc) \
   (((loc)->val.ch) = (char)(chr), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_SHORT(top, num, loc) \
   (((loc)->val.i) = (short)(num), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_INT(top, num, loc) \
   (((loc)->val.i) = (int)(num), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_LONG(top, num, loc) \
   (((loc)->val.i) = (long)(num), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_UNSIGNED_CHAR(top, chr, loc) \
   (((loc)->val.ch) = (unsigned char)(chr), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_UNSIGNED_SHORT(top, num, loc) \
   (((loc)->val.i) = (unsigned short)(num), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_UNSIGNED_INT(top, num, loc) \
   (((loc)->val.i) = (unsigned int)(num), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_UNSIGNED_LONG(top, num, loc) \
   (((loc)->val.i) = (unsigned long)(num), SET_TAG(loc, CL_C_PRIMITIVE))

#define LOAD_C_FLOAT(top, flt, loc) \
   (((loc)->val.fl) = make_float(top, (float)flt), SET_TAG(loc, CL_C_FLOATING))

#define LOAD_C_DOUBLE(top, flt, loc) \
   (((loc)->val.fl) = make_float(top, (double)flt), SET_TAG(loc, CL_C_FLOATING))

#define LOAD_C_STRING(top, str_ptr, loc) \
   (((loc)->val.ch_ptr) = (char *)(str_ptr), SET_TAG(loc, CL_C_STRING))

#define LOAD_C_STRUCT(top, ptr, loc, typesymbol) \
   do {CL_FORM *fld = form_alloc(top, 2); \
       LOAD_SYMBOL(typesymbol, OFFSET(fld, 0)); \
       LOAD_CHAR_PTR(ptr, OFFSET(fld, 1)); \
       ((loc)->val.form) = fld; \
       SET_TAG(loc, CL_C_STRUCT);} while(0)

#define LOAD_C_UNION(top, ptr, loc, typesymbol) \
   do {CL_FORM *fld = form_alloc(top, 2); \
       LOAD_SYMBOL(typesymbol, OFFSET(fld, 0)); \
       LOAD_CHAR_PTR(ptr, OFFSET(fld, 1)); \
       ((loc)->val.form) = fld; \
       SET_TAG(loc, CL_C_UNION);} while(0)

#define LOAD_C_ARRAY(top, ptr, loc, typesymbol) \
   do {CL_FORM *fld = form_alloc(top, 2); \
       LOAD_SYMBOL(typesymbol, OFFSET(fld, 0)); \
       LOAD_CHAR_PTR(ptr, OFFSET(fld, 1)); \
       ((loc)->val.form) = fld; \
       SET_TAG(loc, CL_C_ARRAY);} while(0)

#define LOAD_C_HANDLE(top, ptr, loc, typesymbol) \
   do {CL_FORM *fld = form_alloc(top, 2); \
       LOAD_SYMBOL(typesymbol, OFFSET(fld, 0)); \
       LOAD_CHAR_PTR(ptr, OFFSET(fld, 1)); \
       ((loc)->val.form) = fld; \
       SET_TAG(loc, CL_C_HANDLE);} while(0)


/*------------------------------------------------------------------------------
 * Konvertierungs-Makros von Lisp nach C und von C nach C
 * elementare C-Typen,
 * Lisp-Typen,
 * Zeichenketten
 *----------------------------------------------------------------------------*/
#define MAKE_CL_C_CHAR(top, loc)   LOAD_C_CHAR(top, GET_CHAR(loc), loc)
#define MAKE_CL_C_CHAR_2(top, loc) LOAD_C_CHAR(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_CHAR(top, loc)   LOAD_C_CHAR(top, GET_C_CHAR(loc), loc)

#define MAKE_CL_C_SHORT(top, loc)  LOAD_C_SHORT(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_SHORT(top, loc)  LOAD_C_SHORT(top, GET_C_SHORT(loc), loc)

#define MAKE_CL_C_INT(top, loc)    LOAD_C_INT(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_INT(top, loc)    LOAD_C_INT(top, GET_C_INT(loc), loc)

#define MAKE_CL_C_LONG(top, loc)   LOAD_C_LONG(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_LONG(top, loc)   LOAD_C_LONG(top, GET_C_LONG(loc), loc)

#define MAKE_CL_C_UNSIGNED_CHAR(top, loc)   \
   LOAD_C_UNSIGNED_CHAR(top, GET_CHAR(loc), loc)
#define MAKE_CL_C_UNSIGNED_CHAR_2(top, loc) \
   LOAD_C_UNSIGNED_CHAR(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_UNSIGNED_CHAR(top, loc)   \
   LOAD_C_UNSIGNED_CHAR(top, GET_C_UNSIGNED_CHAR(loc), loc)

#define MAKE_CL_C_UNSIGNED_SHORT(top, loc) \
   LOAD_C_UNSIGNED_SHORT(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_UNSIGNED_SHORT(top, loc) \
   LOAD_C_UNSIGNED_SHORT(top, GET_C_UNSIGNED_SHORT(loc), loc)

#define MAKE_CL_C_UNSIGNED_INT(top, loc) \
   LOAD_C_UNSIGNED_INT(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_UNSIGNED_INT(top, loc) \
   LOAD_C_UNSIGNED_INT(top, GET_C_UNSIGNED_INT(loc), loc)

#define MAKE_CL_C_UNSIGNED_LONG(top, loc) \
   LOAD_C_UNSIGNED_LONG(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_UNSIGNED_LONG(top, loc) \
   LOAD_C_UNSIGNED_LONG(top, GET_C_UNSIGNED_LONG(loc), loc)

#define MAKE_CL_C_FLOAT(top, loc) LOAD_C_FLOAT(top, GET_FLOAT(loc), loc)
#define CAST_CL_C_FLOAT(top, loc) LOAD_C_FLOAT(top, GET_C_FLOAT(loc), loc)

#define MAKE_CL_C_DOUBLE(top, loc) LOAD_C_DOUBLE(top, GET_FLOAT(loc), loc)
#define CAST_CL_C_DOUBLE(top, loc) LOAD_C_DOUBLE(top, GET_C_DOUBLE(loc), loc)

#define MAKE_CL_C_STRING(top, loc, addr) LOAD_C_STRING(top, addr, loc)

/*------------------------------------------------------------------------------
 * Konvertierungsfunktionen von C nach Lisp
 * elementare C-Typen,
 * Zeichenketten
 *----------------------------------------------------------------------------*/
#define MAKE_LISP_CHARACTER(top, loc) SET_TAG(loc, CL_CHAR)
#define MAKE_LISP_INTEGER(top, loc) SET_TAG(loc, CL_FIXNUM)
#define MAKE_LISP_FLOAT(top, loc) SET_TAG(loc, CL_FLOAT)

/*------------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/
#define must_malloc(val) malloc(val)
