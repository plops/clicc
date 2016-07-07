/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *-----------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : Laufzeitsystem: Funktionen des FFI
 *
 * $Revision: 1.12 $
 * $Log: foreign.c,v $
 * Revision 1.12  1993/09/28  15:05:59  pm
 * C-Konstruktorfuntionen nach cginline geschoben.
 *
 * Revision 1.11  1993/08/27  11:52:57  sma
 * unsigned -> unsigned short  in Zeile 379.
 *
 * Revision 1.10  1993/08/27  10:26:16  pm
 * Mehr Castings eingebaut
 *
 * Revision 1.9  1993/08/24  11:21:00  pm
 * Erweiterungen um C-Pointer
 *
 * Revision 1.8  1993/07/22  09:13:35  pm
 * Funktionen zur Verwaltung von C-Strukturen
 *
 * Revision 1.7  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.6  1993/06/04  13:45:45  pm
 * Globale Variable save_stack eingefuegt
 *
 * Revision 1.5  1993/05/31  17:08:07  pm
 * Schreibfehler beseitigt
 *
 * Revision 1.4  1993/05/23  17:53:54  pm
 * alle in C geschriebenen Konstruktor-, Test- und
 * Konvertierungsfunktionen fuer die primitiven Typen implementiert
 *
 * Revision 1.3  1993/05/21  13:55:24  pm
 * c-int in int umbenannt
 *
 * Revision 1.2  1993/05/05  09:10:54  pm
 * einige Funtionen zum Umgang mit c-ints eingebaut
 *
 * Revision 1.1  1993/04/28  14:23:23  pm
 * Initial revision
 *
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include <malloc.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * globale Variable, in der die aktuelle Stackposition beim Aufruf
 * einer Call-Out-Funktion gemerkt wird, damit beim Aufruf einer
 * Call-In-Funktion diese Stelle verwendet werden kann.
 *----------------------------------------------------------------------------*/
CL_FORM *save_stack;

/*------------------------------------------------------------------------------
 * internal_make_c_struct typsymbol cpointer
 *----------------------------------------------------------------------------*/
void internal_make_c_struct (base)
CL_FORM *base;
{
   CL_FORM *ptr;
   
   ptr = form_alloc(base, 3);
   LOAD_SYMBOL(GET_SYMBOL(STACK(base, 0)), OFFSET(ptr, 0));
   LOAD_FIXNUM(0, OFFSET(ptr,1));
   LOAD_CHAR_PTR(GET_CHAR_PTR(STACK(base, 1)), OFFSET(ptr, 2));
   LOAD_C_STRUCT(ptr, STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * internal_c_struct_p object typsymbol
 *----------------------------------------------------------------------------*/
void internal_c_struct_p (base)
CL_FORM *base;
{
   RET_BOOL((TYPE_OF(STACK(base, 0)) == CL_C_STRUCT) && 
            (GET_SYMBOL(OFFSET(GET_FORM(STACK(base, 0)), 0)) == 
             GET_SYMBOL(STACK(base, 1))));
}

/*------------------------------------------------------------------------------
 * internal_make_c_struct_ptr object
 *----------------------------------------------------------------------------*/
void internal_make_c_struct_ptr (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_STRUCT_PTR;
}

/*------------------------------------------------------------------------------
 * internal_c_struct_ptr_p object typsymbol
 *----------------------------------------------------------------------------*/
void internal_c_struct_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL((TYPE_OF(STACK(base, 0)) == CL_C_STRUCT_PTR) && 
            (GET_SYMBOL(OFFSET(GET_FORM(STACK(base, 0)), 0)) == 
             GET_SYMBOL(STACK(base, 1))));
}

/*------------------------------------------------------------------------------
 * internal_get_struct_pointer object
 *----------------------------------------------------------------------------*/
void internal_get_struct_pointer (base)
CL_FORM *base;
{
   LOAD_CHAR_PTR(GET_CHAR_PTR(OFFSET(GET_FORM(STACK(base, 0)), 2)),
                 STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * die make_c_<anything> -Funktionen erzeugen Lispobjekte des gewuenschten Typs
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * make_c_<integer> value
 * +-------------+---------+
 * | C_<integer> | <value> |
 * +-------------+---------+
 *----------------------------------------------------------------------------*/
void make_c_char (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_CHAR;
}

void make_c_short (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_SHORT;
}

void make_c_int (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_INT;
}

void make_c_long (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_LONG;
}

void make_c_unsigned_char (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_UNSIGNED_CHAR;
}

void make_c_unsigned_short (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_UNSIGNED_SHORT;
}

void make_c_unsigned_int (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_UNSIGNED_INT;
}

void make_c_unsigned_long (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_UNSIGNED_LONG;
}

/*------------------------------------------------------------------------------
 * make_c_<float> value
 * +-----------+------+    +---------+
 * | C_<float> | fptr-+--->| <value> |
 * +-----------+------+    +---------+
 *----------------------------------------------------------------------------*/
void make_c_float (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_FLOAT;
}

void make_c_double (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_DOUBLE;
}


void make_c_long_double (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_C_LONG_DOUBLE;
}

/*------------------------------------------------------------------------------
 * Die make_lisp_<anything> -Funktionen konvertieren C-Objekte in Lisp-Objekte.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * make_lisp_character value
 *----------------------------------------------------------------------------*/
void make_lisp_character (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_CHAR;
}

/*------------------------------------------------------------------------------
 * make_lisp_integer value
 *----------------------------------------------------------------------------*/
void make_lisp_integer (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_FIXNUM;
}

/*------------------------------------------------------------------------------
 * make_lisp_float value
 *----------------------------------------------------------------------------*/
void make_lisp_float (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) = CL_FLOAT;
}

/*------------------------------------------------------------------------------
 * make_lisp_string value
 *----------------------------------------------------------------------------*/
void make_lisp_string (base)
CL_FORM *base;
{
   TYPE_OF(STACK(base, 0)) == CL_SMSTR;
}

/*------------------------------------------------------------------------------
 * c_float_p object
 *----------------------------------------------------------------------------*/
void c_float_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_FLOAT);
}

void c_double_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_DOUBLE);
}

void c_long_double_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_LONG_DOUBLE);
}

/*------------------------------------------------------------------------------
 * Die make_c_<anytype>_ptr -Funktionen kopieren den Wert und geben einen
 * Pointer auf die Kopie zurueck.
 *----------------------------------------------------------------------------*/
void make_c_char_ptr (base)
CL_FORM *base;
{
   char *space = malloc(sizeof(char));
   
   *space = GET_C_CHAR(STACK(base, 0));
   LOAD_C_CHAR_PTR(space, STACK(base, 0));
}

void make_c_short_ptr (base)
CL_FORM *base;
{
   short *space = (short *)malloc(sizeof(short));
   
   *space = GET_C_SHORT(STACK(base, 0));
   LOAD_C_SHORT_PTR(space, STACK(base, 0));
}

void make_c_int_ptr (base)
CL_FORM *base;
{
   int *space = (int *)malloc(sizeof(int));
   
   *space = GET_C_INT(STACK(base, 0));
   LOAD_C_INT_PTR(space, STACK(base, 0));
}

void make_c_long_ptr (base)
CL_FORM *base;
{
   long *space = (long *)malloc(sizeof(long));
   
   *space = GET_C_LONG(STACK(base, 0));
   LOAD_C_LONG_PTR(space, STACK(base, 0));
}

void make_c_unsigned_char_ptr (base)
CL_FORM *base;
{
   unsigned char *space = (unsigned char *)malloc(sizeof(unsigned char));
   
   *space = GET_C_UNSIGNED_CHAR(STACK(base, 0));
   LOAD_C_UNSIGNED_CHAR_PTR(space, STACK(base, 0));
}

void make_c_unsigned_short_ptr (base)
CL_FORM *base;
{
   unsigned short *space = (unsigned short *)malloc(sizeof(unsigned short));
   
   *space = GET_C_UNSIGNED_SHORT(STACK(base, 0));
   LOAD_C_UNSIGNED_SHORT_PTR(space, STACK(base, 0));
}

void make_c_unsigned_int_ptr (base)
CL_FORM *base;
{
   unsigned int *space = (unsigned int *)malloc(sizeof(unsigned int));
   
   *space = GET_C_UNSIGNED_INT(STACK(base, 0));
   LOAD_C_UNSIGNED_INT_PTR(space, STACK(base, 0));
}

void make_c_unsigned_long_ptr (base)
CL_FORM *base;
{
   unsigned long *space = (unsigned long *)malloc(sizeof(unsigned long));
   
   *space = GET_C_UNSIGNED_LONG(STACK(base, 0));
   LOAD_C_UNSIGNED_LONG_PTR(space, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * Die c_<anything>_p -Funktionen testen auf den gewuenschten Pointertyp.
 *----------------------------------------------------------------------------*/
void c_char_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_CHAR_PTR);
}

void c_short_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_SHORT_PTR);
}

void c_int_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_INT_PTR);
}

void c_long_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_LONG_PTR);
}

void c_unsigned_char_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_UNSIGNED_CHAR_PTR);
}

void c_unsigned_short_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_UNSIGNED_SHORT_PTR);
}

void c_unsigned_int_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_UNSIGNED_INT_PTR);
}

void c_unsigned_long_ptr_p (base)
CL_FORM *base;
{
   RET_BOOL(TYPE_OF(STACK(base, 0)) == CL_C_UNSIGNED_LONG_PTR);
}

