/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : System-Funktionen: Characters
 *
 * $Revision: 1.11 $
 * $Log: character.c,v $
 * Revision 1.11  1993/08/27  11:48:44  sma
 * cproto-Warnungen wegen mehrzeiligem String beseitigt
 *
 * Revision 1.10  1993/08/26  15:31:07  sma
 * include <string.h> eingefuegt und char_table wieder entfernt.
 *
 * Revision 1.9  1993/08/24  16:20:43  sma
 * Variable char_table eingefügt und etwas aufgeräumt.
 *
 * Revision 1.8  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.7  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.6  1993/02/17  15:29:25  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.5  1993/01/08  09:44:13  hk
 * Namen C_ nach c_.
 *
 * Revision 1.4  1992/11/22  13:58:16  kl
 * strncasecmp in our_strncasecmp umbenannt.
 *
 * Revision 1.3  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.2  1992/07/21  13:49:59  hk
 * Fchareq --> FcharE, etc..
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *
 * 13.01.92 : strncasecmp selbst kodiert
 *
 * 25.06.91 : strnicmp durch strncasecmp ersetzt
 *
 * 12.11.90 : Erste Version
 *----------------------------------------------------------------------------*/

#include <string.h>
#include <ctype.h>
#include <c_decl.h>
#include "sys.h"


/*------------------------------------------------------------------------------
 * Fehlermeldungen
 *----------------------------------------------------------------------------*/
char No_char[] = "~a is not a character";


/*------------------------------------------------------------------------------
 * Globale Variablen
 *----------------------------------------------------------------------------*/

char *char_name[] =
{  /* Standard-Character */
   /* ------------------ */
   "Space", "Newline",
   
   /* Semi-Standard Character */
   /* ----------------------- */
   "Backspace", "Tab", "Linefeed", "Page", "Return", "Rubout", NULL };

char char_intern[] = { ' ', '\n', '\b', '\t', '\n', '\f', '\r', '\177' };

/* Dieser String muß in einer Zeile stehen, damit cproto keine Fehler anzeigt */
static char standard_char[] =
" \n!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

char digit_char[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";


/*------------------------------------------------------------------------------
 * Prueft auf Typ CHARACTER
 * Rueckgabewert: Der eigentliche Character der CL_FORM
 *----------------------------------------------------------------------------*/
char check_char(base)
CL_FORM *base;
{
   if(NOT(CL_CHARP(base)))
   {
      Lerror(STACK(base, 0), No_char);
   }
   return(GET_CHAR(base));
}


/*------------------------------------------------------------------------------
 * Wandelt einen Kleinbuchstaben in einen Grossbuchstaben um.
 * Sonstige Zeichen werden nicht veraendert.
 * 'toupper' ist in SYS 5 und BSD 4.3 unterschiedlich definiert.
 *----------------------------------------------------------------------------*/
char save_toupper(ch)
int ch;
{
   return (char)(islower(ch) ? toupper(ch) : ch);
}


/*----------------------------------------------------------------------------*/
/* 13.1. Character Attributes                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* CHAR-CODE-LIMIT = 256                                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* 13.2. Predicates on Characters                                             */
/*----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * standard-char-p char
 *----------------------------------------------------------------------------*/
void Fstandard_char_p(base)
CL_FORM *base;
{
   RET_BOOL(strchr(standard_char, check_char(STACK(base, 0))));
}


/*------------------------------------------------------------------------------
 * graphic-char-p char
 *----------------------------------------------------------------------------*/
void Fgraphic_char_p(base)
CL_FORM *base;
{
   char ch = check_char(STACK(base, 0));
   /* Alle Standardzeichen bis auf #\Newline sind Graphikzeichen */
   RET_BOOL(ch != '\n' && strchr(standard_char, ch));
}


/*------------------------------------------------------------------------------
 * string-char-p char
 *----------------------------------------------------------------------------*/
void Fstring_char_p(base)
CL_FORM *base;
{
   check_char(STACK(base, 0));
   LOAD_T(STACK(base, 0));    /* Alle Zeichen koennen in Strings vorkommen */
}


/*------------------------------------------------------------------------------
 * alpha-char-p char
 *----------------------------------------------------------------------------*/
void Falpha_char_p(base)
CL_FORM *base;
{
   RET_BOOL(isalpha(check_char(STACK(base, 0))));
}


/*------------------------------------------------------------------------------
 * upper-case-p char
 *----------------------------------------------------------------------------*/
void Fupper_case_p(base)
CL_FORM *base;
{
   RET_BOOL(isupper(check_char(STACK(base, 0))));
}


/*------------------------------------------------------------------------------
 * lower-case-p char
 *----------------------------------------------------------------------------*/
void Flower_case_p(base)
CL_FORM *base;
{
   RET_BOOL(islower(check_char(STACK(base, 0))));
}


/*------------------------------------------------------------------------------
 * both-case-p char
 *----------------------------------------------------------------------------*/
void Fboth_case_p(base)
CL_FORM *base;
{
   RET_BOOL(isalpha(check_char(STACK(base, 0))));
}


/*------------------------------------------------------------------------------
 * digit-char-p-internal char radix
 * ACHTUNG: Funktioniert so nicht für EBCDI !
 *----------------------------------------------------------------------------*/
void digit_char_p_internal(base)
CL_FORM *base;
{
   char ch    = check_char(STACK(base, 0)); /* Zeichen selektieren */
   long radix = GET_FIXNUM(STACK(base, 1));

   if(isdigit(ch))
   {
      ch -= '0';
   }
   else if(islower(ch))
   {
      ch -= ('a' - 10);
   }
   else if(isupper(ch))
   {
      ch -= ('A' - 10);
   }
   else
   {
      LOAD_NIL(STACK(base, 0));
      return;
   }

   if(ch < radix)
   {
      LOAD_FIXNUM(ch, STACK(base, 0));
   }
   else
   {
      LOAD_NIL(STACK(base, 0));
   }
}


/*------------------------------------------------------------------------------
 * alphanumericp char
 *----------------------------------------------------------------------------*/
void Falphanumericp(base)
CL_FORM *base;
{
   RET_BOOL(isalnum(check_char(STACK(base, 0))));
}


/*------------------------------------------------------------------------------
 * char= character &REST more-characters
 *----------------------------------------------------------------------------*/
void FcharE(base, nargs)
CL_FORM *base;
int nargs;
{
   char ch;
   int i;

   ch = check_char(STACK(base, 0)); /* 1. Zeichen selektieren */

	/* Typueberpruefung auf CL_CHAR und Vergleich mit dem 1. Zeichen.
    * Beim ersten Zeichen, welches ungleich dem 1. Zeichen ist,
    * Abbruch und als Ergebnis NIL liefern.
    */
   for(i = 1; i < nargs; i++)
   {
      if(ch != check_char(STACK(base, i)))
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * CHAR/= character &REST more-characters
 *----------------------------------------------------------------------------*/
void FcharNE(base, nargs)
CL_FORM *base;
int nargs;
{
   int  i, j;
   char ch;

   for(i = 0; i < nargs; i++)
   {
      ch = check_char(STACK(base, i)); /* Typueberpruefung des i-ten Args */

      /* Vergleich mit allen Argumenten j > i */
      for(j = i + 1; j < nargs; j++)
      {
         if(ch == check_char(STACK(base, j)))
         {
            LOAD_NIL(STACK(base, 0));
            return;
         }
      }
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char< character &REST more-characters
 *----------------------------------------------------------------------------*/
void FcharL(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = check_char(STACK(base, 0));
   for(i = 1; i < nargs; i++)
   {
      next_ch = check_char(STACK(base, i));
      if(last_ch >= next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char> character &REST more-characters
 *----------------------------------------------------------------------------*/
void FcharG(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = check_char(STACK(base, 0));
   for(i = 1; i < nargs; i++)
   {
      next_ch = check_char(STACK(base, i));
      if(last_ch <= next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char<= character &REST more-characters
 *----------------------------------------------------------------------------*/
void FcharLE(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = check_char(STACK(base, 0));
   for(i = 1; i < nargs; i++)
   {
      next_ch = check_char(STACK(base, i));
      if(last_ch > next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char>= character &REST more-characters
 *----------------------------------------------------------------------------*/
void FcharGE(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = check_char(STACK(base, 0));
   for(i = 1; i < nargs; i++)
   {
      next_ch = check_char(STACK(base, i));
      if(last_ch < next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-equal character &rest more-characters
 *----------------------------------------------------------------------------*/
void Fchar_equal(base, nargs)
CL_FORM *base;
int nargs;
{
   char ch;
   int i;

	/* 1. Zeichen selektieren und in Grossbuchstaben umwandeln */
   ch = save_toupper(check_char(STACK(base, 0)));

   for(i = 1; i < nargs; i++)
   {
      /* Typueberpruefung auf CL_CHAR und Vergleich mit dem 1. Zeichen.
       * Durch Umwandlung in Grossbuchstaben wird gross/klein ignoriert.
       * Beim ersten Zeichen, welches ungleich dem 1. Zeichen ist,
       * Abbruch und als Ergebnis NIL liefern.
       */
      if(ch != save_toupper(check_char(STACK(base, i))))
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-not-equal character &rest more-characters
 *----------------------------------------------------------------------------*/
void Fchar_not_equal(base, nargs)
CL_FORM *base;
int nargs;
{
   int  i, j;
   char ch;

   for(i = 0; i < nargs; i++)
   {
		/* Typueberpruefung des i-ten Args */
      ch = save_toupper(check_char(STACK(base, i)));

		/* Vergleich mit allen Argumenten j > i */
      for(j = i + 1; j < nargs; j++)
      {
         if(ch == save_toupper(check_char(STACK(base, j))))
         {
            LOAD_NIL(STACK(base, 0));
            return;
         }
      }
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-lessp character &rest more-characters
 *----------------------------------------------------------------------------*/
void Fchar_lessp(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = save_toupper(check_char(STACK(base, 0)));
   for(i = 1; i < nargs; i++)
   {
      next_ch = save_toupper(check_char(STACK(base, i)));
      if(last_ch >= next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-greaterp character &rest more-characters
 *----------------------------------------------------------------------------*/
void Fchar_greaterp(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = save_toupper(check_char(STACK(base, 0)));
   for(i = 1; i < nargs; i++)
   {
      next_ch = save_toupper(check_char(STACK(base, i)));
      if(last_ch <= next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-not-greaterp character &rest more-characters
 *----------------------------------------------------------------------------*/
void Fchar_not_greaterp(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = save_toupper(check_char(STACK(base, 0)));
   for(i = 1; i < nargs; i++)
   {
      next_ch = save_toupper(check_char(STACK(base, i)));
      if(last_ch > next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-not-lessp character &rest more-characters
 *----------------------------------------------------------------------------*/
void Fchar_not_lessp(base, nargs)
CL_FORM *base;
int nargs;
{
   char last_ch, next_ch;
   int  i;

   last_ch = save_toupper(check_char(STACK(base, 0)));
   for(i = 1; i < nargs; i++)
   {
      next_ch = save_toupper(check_char(STACK(base, i)));
      if(last_ch < next_ch)
      {
         LOAD_NIL(STACK(base, 0));
         return;
      }
      last_ch = next_ch;
   }
   LOAD_T(STACK(base, 0));
}


/*----------------------------------------------------------------------------*/
/* 13.3. Character Construction and Selection                                 */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * char-code char
 *----------------------------------------------------------------------------*/
void Fchar_code(base)
CL_FORM *base;
{
   char ch = check_char(STACK(base, 0));

   LOAD_FIXNUM(ch, STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * code-char-internal code                                   [CL2]
 *----------------------------------------------------------------------------*/
void code_char_internal(base)
CL_FORM *base;
{
   char code = GET_FIXNUM(STACK(base, 0));

   LOAD_CHAR(code, STACK(base, 0));
}


/*----------------------------------------------------------------------------*/
/* 13.4. Character Conversions                                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* CHARACTER object                                                           */
/*    -> COERCE object 'CHARACTER                                             */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * char-upcase char
 *----------------------------------------------------------------------------*/
void Fchar_upcase(base)
CL_FORM *base;
{
   char upper = save_toupper(check_char(STACK(base, 0)));
   LOAD_CHAR(upper, STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-downcase char
 *----------------------------------------------------------------------------*/
void Fchar_downcase(base)
CL_FORM *base;
{
   char lower = check_char(STACK(base, 0));
   if(isupper(lower))
      lower = tolower(lower);
   LOAD_CHAR(lower, STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * digit-char-internal weight radix                          [CL2]
 *----------------------------------------------------------------------------*/
void digit_char_internal(base)
CL_FORM *base;
{
   long weight = GET_FIXNUM(STACK(base, 0));
   LOAD_CHAR(digit_char[weight], STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * char-int char
 *----------------------------------------------------------------------------*/
void Fchar_int(base)
CL_FORM *base;
{
   char ch = check_char(STACK(base, 0));

   LOAD_FIXNUM(ch, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * char-name char
 *----------------------------------------------------------------------------*/
void Fchar_name(base)
CL_FORM *base;
{
   int index;

   switch(check_char(STACK(base, 0)))
   {
   case ' ' : index = 0; break;
   case '\n': index = 1; break;
   case '\b': index = 2; break;
   case '\t': index = 3; break;
   case '\f': index = 5; break;
   case '\r': index = 6; break;
   default: LOAD_NIL(STACK(base, 0)); return;
   }
   make_string(STACK(base, 0), char_name[index]);
}


/*------------------------------------------------------------------------------
 * our_strncasecmp str1 str2 len
 *----------------------------------------------------------------------------*/
int our_strncasecmp(str1, str2, len)
char *str1, *str2;
int len;
{
   int diff;

   while(len--)
   {
      if(*str1 == '\0')
         return(-(*str2 != '\0'));
      if(*str2 == '\0')
         return(1);

      diff = save_toupper(*str1) - save_toupper(*str2);
      if(diff)
         return((diff > 0) ? 1 : -1);
      else
      {
         str1++;
         str2++;
      }
   }
   return(0);
}


/*------------------------------------------------------------------------------
 * name-char name
 *----------------------------------------------------------------------------*/
void Fname_char(base)
CL_FORM *base;
{
   extern Fstring();
   char *string;
   long size;
   int i = 0;

   Fstring(STACK(base, 0));   /* (STRING name) */
   size   = AR_SIZE(GET_FORM(STACK(base, 0)));
   string = get_c_string(STACK(base, 0));
   while(char_name[i])
   {
      if(! our_strncasecmp(string, char_name[i], size))
      {
         LOAD_CHAR(char_intern[i], STACK(base, 0));
         return;
      }
      i++;
   }
   LOAD_NIL(STACK(base, 0));
}

