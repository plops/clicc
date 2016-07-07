/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : Laufzeitsystem
 *            - Low Level Hash-Funktionen
 *
 * $Revision: 1.8 $
 * $Log: hash.c,v $
 * Revision 1.8  1993/06/30  16:41:45  hk
 * Neue Funktionen: sxhash_simple_string, sxhash_string, combine_hash.
 *
 * Revision 1.7  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.6  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.5  1993/02/17  15:42:20  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.4  1993/01/08  09:44:13  hk
 * Namen C_ nach c_.
 *
 * Revision 1.3  1992/10/05  17:00:26  hk
 * Kommentar zu string-hash korrigiert.
 *
 * Revision 1.2  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * Berechnet einen Hash-Index (unsigned long) aus den Zeichen eines Strings
 * angegebener Laenge.
 *----------------------------------------------------------------------------*/
unsigned long hash(str, len)
char *str;
long len;
{
   unsigned char c;
   unsigned long ret = 0;
   
   for( ; len > 0; len--)
   {
      c = *str++;
      ret <<= 4;
      ret += c;
   }
   return(ret);
}

/*------------------------------------------------------------------------------
 * string_hash string hash-size
 * berechnet Fixnum aus string, 0 <= Resultat < hash-size
 *----------------------------------------------------------------------------*/
void string_hash(base)
CL_FORM *base;
{
   LOAD_FIXNUM(hash(get_c_string(STACK(base, 0)),
                    AR_SIZE(GET_FORM(STACK(base, 0))))
               % GET_FIXNUM(STACK(base, 1)),
               STACK(base, 0) ); 
}

/*------------------------------------------------------------------------------
 * sxhash-simple-string simple-string
 * Berechnet einen Hash-Index (Fixnum) aus einem simple String
 *----------------------------------------------------------------------------*/
void sxhash_simple_string(base)
CL_FORM *base;
{
   LOAD_FIXNUM(hash(sm_get_c_string(STACK(base, 0)),
                    AR_SIZE(GET_FORM(STACK(base, 0)))),
               STACK(base, 0)); 
}

/*------------------------------------------------------------------------------
 * sxhash-string string
 * Berechnet einen Hash-Index (Fixnum) aus einem String
 *----------------------------------------------------------------------------*/
void sxhash_string(base)
CL_FORM *base;
{
   LOAD_FIXNUM(hash(get_c_string(STACK(base, 0)),
                    AR_SIZE(GET_FORM(STACK(base, 0)))),
               STACK(base, 0)); 
}

/*------------------------------------------------------------------------------
 * combine-hash hash1 hash2
 * Kombiniert zwei Hash-Indizes zu einem neuen Hash-Index
 *----------------------------------------------------------------------------*/
void combine_hash(base)
CL_FORM *base;
{
   LOAD_FIXNUM(GET_FIXNUM(STACK(base, 0)) ^ GET_FIXNUM(STACK(base, 1)),
               STACK(base, 0)); 
}
