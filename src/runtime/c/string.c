/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : System-Funktionen: Strings

 * $Revision: 1.8 $
 * $Log: string.c,v $
 * Revision 1.8  1993/10/13  18:16:01  sma
 * 1x TYPE_OF durch CL_STRINGP ersetzt und das Setzen von AR_SIZE in
 * make_string_internal korrigiert.
 *
 * Revision 1.7  1993/08/26  15:58:21  hk
 * make_string_internal: Typ void
 *
 * Revision 1.6  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.5  1993/05/08  18:17:56  hk
 * Argumentreihenfolge von set-char-internal und set-schar-internal
 * geaendert.
 *
 * Revision 1.4  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.3  1993/02/17  15:46:30  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.2  1993/01/08  09:44:13  hk
 * Namen C_ nach c_.
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *----------------------------------------------------------------------------*/

#include <string.h>
#include <ctype.h>
#include <c_decl.h>
#include "sys.h"

/*----------------------------------------------------------------------------
 * Ermittelt den 'eigentlichen' String eines LISP-Strings
 * Resultat: Zeiger auf den String
 *----------------------------------------------------------------------------*/
char *get_c_string (lisp_string)
CL_FORM *lisp_string;
{
   CL_FORM *header = GET_FORM (lisp_string);

   if (CL_STRINGP(lisp_string) && DISPLACED_P (header))
      return (get_c_string (DISPLACED_TO (header)));
   else
      return (GET_CHAR_PTR (AR_BASE (header))); 
}

/*----------------------------------------------------------------------------
 * Ermittelt den 'eigentl.' String eines Simple-LISP-Strings
 * Resultat: Zeiger auf den String
 *----------------------------------------------------------------------------*/
char *sm_get_c_string (lisp_string)
CL_FORM *lisp_string;
{
   CL_FORM *header = GET_FORM (lisp_string);

   return (GET_CHAR_PTR (AR_BASE (header))); 
}

/*----------------------------------------------------------------------------
 * Erzeugt aus einem C-String einen LISP-String nach 'base'
 *----------------------------------------------------------------------------*/
void make_string (base, string)
CL_FORM *base;
char    *string;
{
   long     str_len = strlen (string);
   CL_FORM *header;
   char    *chptr;

   header = form_alloc (STACK (base, 0), 2L);
   chptr  = char_alloc (STACK (base, 0), str_len);
   strncpy (chptr, string, str_len);
   SET_AR_SIZE (str_len, header);
   LOAD_CHAR_PTR (chptr, AR_BASE (header));
   LOAD_SMSTR (header, STACK (base, 0));
}

/*----------------------------------------------------------------------------
 * 18.1. String Access
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * CHAR-INTERNAL string index
 *----------------------------------------------------------------------------*/
void char_internal (base)
CL_FORM *base;
{
   char *string = get_c_string (STACK (base, 0));
   int   index  = GET_FIXNUM   (STACK (base, 1));

   /* Fill-Pointer werden nicht beruecksichtigt. */
   /* ------------------------------------------ */
   LOAD_CHAR (string[index], STACK (base, 0));
}

/*----------------------------------------------------------------------------
 * SET-CHAR-INTERNAL character string index
 *----------------------------------------------------------------------------*/
void set_char_internal (base)
CL_FORM *base;
{
   char *string = get_c_string (STACK (base, 1));
   int   index  = GET_FIXNUM   (STACK (base, 2));

   /* Fill-Pointer werden nicht beruecksichtigt. */
   /* ------------------------------------------ */
   string[index] = GET_CHAR (STACK (base, 0));
}

/*----------------------------------------------------------------------------
 * SCHAR-INTERNAL simple-string index
 *----------------------------------------------------------------------------*/
void schar_internal (base)
CL_FORM *base;
{
   char *string = sm_get_c_string (STACK (base, 0));
   int   index  = GET_FIXNUM      (STACK (base, 1));

   /* Fill-Pointer werden nicht beruecksichtigt. */
   /* ------------------------------------------ */
   LOAD_CHAR (string[index], STACK (base, 0));
}

/*----------------------------------------------------------------------------
 * SET-SCHAR-INTERNAL character simple-string index
 *----------------------------------------------------------------------------*/
void set_schar_internal (base)
CL_FORM *base;
{
   char *string = sm_get_c_string (STACK (base, 1));
   int   index  = GET_FIXNUM      (STACK (base, 2));

   /* Fill-Pointer werden nicht beruecksichtigt. */
   /* ------------------------------------------ */
   string[index] = GET_CHAR (STACK (base, 0));
}

/*----------------------------------------------------------------------------
 * 18.3. String Construction and Manipulation
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * MAKE-STRING-INTERNAL size initial-element
 *----------------------------------------------------------------------------*/
void make_string_internal (base)
CL_FORM *base;
{
   long size      = GET_FIXNUM (STACK (base, 0));
   char init_elem = GET_CHAR   (STACK (base, 1));
   char *new_string;
   int i;
   CL_FORM *sm_string;

   /* Neuen String allozieren und initialisieren */
   /* ------------------------------------------ */
   new_string = char_alloc (STACK (base, 2), size);
   memset(new_string, init_elem, size);

   /* Header fuer SIMPLE-STRING allozieren */
   /* ------------------------------------ */
   sm_string = form_alloc (STACK (base, 2), 2L);
   SET_AR_SIZE(size, sm_string);
   LOAD_CHAR_PTR (new_string, AR_BASE (sm_string));

   LOAD_SMSTR (sm_string, STACK (base, 0));
}
