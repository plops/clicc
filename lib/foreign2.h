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
 * $Id: foreign2.h,v 1.7 1995/03/06 17:47:10 wg Exp $
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Datentypen-Definition
 *----------------------------------------------------------------------------*/
#define CL_C_PRIMITIVE       SIZE_TAG(CL_FOREIGN, 0)
#define CL_C_FLOATING        SIZE_TAG(CL_FOREIGN, 1)
#define CL_C_STRING          SIZE_TAG(CL_FOREIGN, 2)

#define CL_C_STRUCT          SIZE_TAG(CL_FOREIGN, 3)
#define CL_C_UNION           SIZE_TAG(CL_FOREIGN, 4)
#define CL_C_ARRAY           SIZE_TAG(CL_FOREIGN, 5)
#define CL_C_HANDLE          SIZE_TAG(CL_FOREIGN, 6)


/*------------------------------------------------------------------------------
 * Makros für den Zugriff auf die Komponenten eines FOREIGN-Objektes
 *----------------------------------------------------------------------------*/
/*--- Allgemeine Makros ---*/
#define GET_C_INTEGER(loc)        ((loc)->form[1].d)
#define GET_C_CHARACTER(loc)      ((loc)->form[1].d)
#define GET_C_FLOATING(loc)       (*((loc)->form[1].fl))
#define GET_C_FLOATING_PTR(loc)   (double *)((loc)->form[1].fl)

#define GET_C_CHAR(loc)           (char)GET_C_CHARACTER(loc)
#define GET_C_SHORT(loc)          (short)GET_C_INTEGER(loc)
#define GET_C_INT(loc)            (int)GET_C_INTEGER(loc)
#define GET_C_LONG(loc)           (long)GET_C_INTEGER(loc)
#define GET_C_UNSIGNED_CHAR(loc)  (unsigned char)GET_C_CHARACTER(loc)
#define GET_C_UNSIGNED_SHORT(loc) (unsigned short)GET_C_INTEGER(loc)
#define GET_C_UNSIGNED_INT(loc)   (unsigned int)GET_C_INTEGER(loc)
#define GET_C_UNSIGNED_LONG(loc)  (unsigned long)GET_C_INTEGER(loc)
#define GET_C_FLOAT(loc)          (float)GET_C_FLOATING(loc)
#define GET_C_DOUBLE(loc)         (double)GET_C_FLOATING(loc)

#define GET_C_STRING(loc)         (char *)((loc)->form[1].str)

#define GET_C_TYPESYMBOL(loc)     ((loc)->form[1].form)

#define GET_C_STRUCT_PTR(loc)     (void *)((loc)->form[2].str)
#define GET_C_UNION_PTR(loc)      (void *)((loc)->form[2].str)
#define GET_C_ARRAY_PTR(loc)      (void *)((loc)->form[2].str)
#define GET_C_HANDLE(loc)         (void *)((loc)->form[2].str)

/*------------------------------------------------------------------------------
 * Typtest-Prädikate
 *----------------------------------------------------------------------------*/
#define F_TAG_FIELD(loc)           ((loc)[0].d)
#define F_TYPE_OF(loc)             F_TAG_FIELD((loc)->form)

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
/*--- Allgemeine Makros ---*/
#define LOAD_C_INTEGER(top, val, loc) \
do {CL_FORM *fld = form_alloc(top, 2); \
       fld[0].d = CL_C_PRIMITIVE; \
       fld[1].d = val; \
       (loc)->form = fld;} while(0)

#define LOAD_C_FLOATING(top, val, loc) \
do {CL_FORM *fld = form_alloc(top, 2); \
       fld[0].d = CL_C_FLOATING; \
       fld[1].fl = make_float(top, val); \
       (loc)->form = fld;} while(0)

#define LOAD_C_STRING(top, val, loc) \
do {CL_FORM *fld = form_alloc(top, 2); \
       fld[0].d = CL_C_STRING; \
       fld[1].str = val; \
       (loc)->form = fld;} while(0)

#define LOAD_C_CHAR(top, val, loc) \
  LOAD_C_INTEGER(top, (char)val, loc)
         
#define LOAD_C_SHORT(top, val, loc) \
  LOAD_C_INTEGER(top, (short)val, loc)

#define LOAD_C_INT(top, val, loc) \
  LOAD_C_INTEGER(top, (int)val, loc)

#define LOAD_C_LONG(top, val, loc) \
  LOAD_C_INTEGER(top, (long)val, loc)

#define LOAD_C_UNSIGNED_CHAR(top, val, loc) \
  LOAD_C_INTEGER(top, (unsigned char)val, loc)

#define LOAD_C_UNSIGNED_SHORT(top, val, loc) \
  LOAD_C_INTEGER(top, (unsigned short)val, loc)

#define LOAD_C_UNSIGNED_INT(top, val, loc) \
  LOAD_C_INTEGER(top, (unsigned int)val, loc)

#define LOAD_C_UNSIGNED_LONG(top, val, loc) \
  LOAD_C_INTEGER(top, (unsigned long)val, loc)

#define LOAD_C_FLOAT(top, val, loc) \
  LOAD_C_FLOATING(top, (float)val, loc)

#define LOAD_C_DOUBLE(top, val, loc) \
  LOAD_C_FLOATING(top, (double)val, loc)

#define LOAD_C_STRUCT(top, ptr, loc, typesymbol) \
do {CL_FORM *fld = form_alloc(top, 3); \
       fld[0].d = CL_C_STRUCT; \
       fld[1].form = typesymbol; \
       fld[2].str = ptr; \
       (loc)->form = fld;} while(0)

#define LOAD_C_UNION(top, ptr, loc, typesymbol) \
do {CL_FORM *fld = form_alloc(top, 3); \
       fld[0].d = CL_C_UNION; \
       fld[1].form = typesymbol; \
       fld[2].str = ptr; \
       (loc)->form = fld;} while(0)

#define LOAD_C_ARRAY(top, ptr, loc, typesymbol) \
do {CL_FORM *fld = form_alloc(top, 3); \
       fld[0].d = CL_C_ARRAY; \
       fld[1].form = typesymbol; \
       fld[2].str = ptr; \
       (loc)->form = fld;} while(0)

#define LOAD_C_HANDLE(top, ptr, loc, typesymbol) \
do {CL_FORM *fld = form_alloc(top, 3); \
       fld[0].d = CL_C_HANDLE; \
       fld[1].form = typesymbol; \
       fld[2].str = ptr; \
       (loc)->form = fld;} while(0)
     

/*------------------------------------------------------------------------------
 * Konvertierungsfunktionen von Lisp nach C und C nach C
 * elementare C-Typen,
 * Lisp-Typen,
 * Zeichenketten
 *----------------------------------------------------------------------------*/
/*--- c-char ---*/
#define MAKE_CL_C_CHAR(top, loc)   LOAD_C_CHAR(top, GET_CHAR(loc), loc)
#define MAKE_CL_C_CHAR_2(top, loc) LOAD_C_CHAR(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_CHAR(top, loc)   LOAD_C_CHAR(top, GET_C_INTEGER(loc), loc)

/*--- c-short ---*/
#define MAKE_CL_C_SHORT(top, loc) LOAD_C_SHORT(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_SHORT(top, loc) LOAD_C_SHORT(top, GET_C_INTEGER(loc), loc)

/*--- c-int ---*/
#define MAKE_CL_C_INT(top, loc) LOAD_C_INT(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_INT(top, loc) LOAD_C_INT(top, GET_C_INTEGER(loc), loc)

/*--- c-long ---*/
#define MAKE_CL_C_LONG(top, loc) LOAD_C_LONG(top, GET_FIXNUM(loc), loc)
#define CAST_CL_C_LONG(top, loc) LOAD_C_LONG(top, GET_C_INTEGER(loc), loc)

/*--- c-unsigned-char ---*/
#define MAKE_CL_C_UNSIGNED_CHAR(top, loc) \
   LOAD_C_UNSIGNED_CHAR(top, GET_CHAR(loc), loc)

#define MAKE_CL_C_UNSIGNED_CHAR_2(top, loc) \
   LOAD_C_UNSIGNED_CHAR(top, GET_FIXNUM(loc), loc)
  
#define CAST_CL_C_UNSIGNED_CHAR(top, loc) \
  LOAD_C_UNSIGNED_CHAR(top, GET_C_INTEGER(loc), loc)

/*--- c-unsigned-short ---*/
#define MAKE_CL_C_UNSIGNED_SHORT(top, loc) \
   LOAD_C_UNSIGNED_SHORT(top, GET_FIXNUM(loc), loc)

#define CAST_CL_C_UNSIGNED_SHORT(top, loc) \
   LOAD_C_UNSIGNED_SHORT(top, GET_C_INTEGER(loc), loc)

/*--- c-unsigned-int ---*/
#define MAKE_CL_C_UNSIGNED_INT(top, loc) \
   LOAD_C_UNSIGNED_INT(top, GET_FIXNUM(loc), loc)

#define CAST_CL_C_UNSIGNED_INT(top, loc) \
   LOAD_C_UNSIGNED_INT(top, GET_C_INTEGER(loc), loc)

/*--- c-unsigned-long ---*/
#define MAKE_CL_C_UNSIGNED_LONG(top, loc) \
   LOAD_C_UNSIGNED_LONG(top, GET_FIXNUM(loc), loc)

#define CAST_CL_C_UNSIGNED_LONG(top, loc) \
   LOAD_C_UNSIGNED_LONG(top, GET_C_INTEGER(loc), loc)

/*--- c-float ---*/
#define MAKE_CL_C_FLOAT(top, loc) \
   LOAD_C_FLOAT(top, GET_FLOAT(loc), loc)

#define CAST_CL_C_FLOAT(top, loc) \
   LOAD_C_FLOAT(top, GET_C_FLOATING(loc), loc)

/*--- c-double ---*/
#define MAKE_CL_C_DOUBLE(top, loc) \
   LOAD_C_DOUBLE(top, GET_FLOAT(loc), loc)

#define CAST_CL_C_DOUBLE(top, loc) \
   LOAD_C_DOUBLE(top, GET_C_FLOATING(loc), loc)

/*--- c-string ---*/
#define MAKE_CL_C_STRING(top, loc, addr) \
   LOAD_C_STRING(top, addr, loc)


/*------------------------------------------------------------------------------
 * Konvertierungsfunktionen von C nach Lisp
 * elementare C-Typen,
 * Zeichenketten
 *----------------------------------------------------------------------------*/
#define MAKE_LISP_CHARACTER(top, loc) LOAD_CHAR(top, GET_C_INTEGER(loc), loc)
#define MAKE_LISP_INTEGER(top, loc) LOAD_FIXNUM(top, GET_C_INTEGER(loc), loc)
#define MAKE_LISP_FLOAT(top, loc) LOAD_FLOAT(top, GET_C_FLOATING_PTR(loc), loc)

/*------------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/
#define must_malloc(val) malloc(val)
