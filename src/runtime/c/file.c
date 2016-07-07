/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *---------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : Laufzeitsystem
 *            - c_fopen
 *            - c_fclose
 *            - c_ftell
 *            - c_fseek
 *            - c_file_length
 *            - c_stdin
 *            - c_stdout
 *            - c_fgetc
 *            - c_fputc
 *            - c_ungetc
 *
 * $Revision: 1.9 $
 * $Log: file.c,v $
 * Revision 1.9  1993/10/13  16:54:29  sma
 * Da LOAD_CFILE zu einem Block expandieren kann, muß es bei if/then/else
 * in { } stehen.
 *
 * Revision 1.8  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.7  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.6  1993/03/25  12:52:44  sma
 * neuer Look
 *
 * Revision 1.5  1993/02/17  15:36:34  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.4  1993/01/08  09:44:13  hk
 * Namen C_ nach c_.
 *
 * Revision 1.3  1993/01/06  16:29:51  hk
 * C-fopen vereinfacht, da nun Lisp-Strings als C-strings verwendet
 * werden koennen.
 *
 * Revision 1.2  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *--------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"


char NOT_UNREAD[] = "Could not unread character ~s";

/*-------------------------------------------------------------------------
 * C-open name mode
 *-------------------------------------------------------------------------*/
void 
c_fopen(base)
CL_FORM *base;
{
   FILE *f = fopen(get_c_string(STACK(base, 0)), get_c_string(STACK(base, 1)));

   if (f != NULL)
   {
      LOAD_CFILE(f, STACK(base, 0));
   }
   else
      LOAD_NIL(STACK(base, 0));
}

/*-------------------------------------------------------------------------
 * C-fclose cfile
 *-------------------------------------------------------------------------*/
void 
c_fclose(base)
CL_FORM *base;
{
   fclose(GET_CFILE(STACK(base, 0)));
}

/*-------------------------------------------------------------------------
 * C-ftell cfile
 *-------------------------------------------------------------------------*/
void 
c_ftell(base)
CL_FORM *base;
{
   LOAD_FIXNUM(ftell(GET_CFILE(STACK(base, 0))), STACK(base, 0));
}

/*-------------------------------------------------------------------------
 * C-fseek cfile offset
 *-------------------------------------------------------------------------*/
void 
c_fseek(base)
CL_FORM *base;
{
   fseek(GET_CFILE(STACK(base, 0)), GET_FIXNUM(STACK(base, 1)), 0);
}

/*-------------------------------------------------------------------------
 * C-file-length cfile
 *-------------------------------------------------------------------------*/
void 
c_file_length(base)
CL_FORM *base;
{
   FILE *f = GET_CFILE(STACK(base, 0));
   long pos = ftell(f);
   long len;

   /* Dateiende */
   fseek(f, 0L, 2);
   len = ftell(f);
   fseek(f, pos, 0);
   LOAD_FIXNUM(len, STACK(base, 0));
}

/*-------------------------------------------------------------------------
 * get stdin handle
 *-------------------------------------------------------------------------*/
void 
c_stdin(base)
CL_FORM *base;
{
   LOAD_CFILE(stdin, STACK(base, 0));
}

/*-------------------------------------------------------------------------
 * get stdout handle
 *-------------------------------------------------------------------------*/
void 
c_stdout(base)
CL_FORM *base;
{
   LOAD_CFILE(stdout, STACK(base, 0));
}

/*-------------------------------------------------------------------------
 * c-fgetc cfile
 *-------------------------------------------------------------------------*/
#if 1
void 
c_fgetc(base)
CL_FORM *base;
{
   int c = fgetc(GET_CFILE(STACK(base, 0)));

   if (c == EOF)
      LOAD_NIL(STACK(base, 0));
   else
   {
      LOAD_CHAR(c, STACK(base, 0));
   }
}
#else
void 
c_fgetc(base)
CL_FORM *base;
{
   static char buf[256];
   static char *ptr = buf;

   if (*ptr == '\0')
   {
      if (!fgets(buf, sizeof(buf), GET_CFILE(STACK(base, 0))))
      {
	 LOAD_NIL(STACK(base, 0));
	 buf[0] = '\0';
	 ptr = buf;
	 return;
      }
      else
      {
	 ptr = buf;
      }
   }
   LOAD_CHAR(*ptr++, STACK(base, 0));
}
#endif

/*-------------------------------------------------------------------------
 * C-fputc char cfile
 *-------------------------------------------------------------------------*/
void 
c_fputc(base)
CL_FORM *base;
{
   /* Aufruf erfolgt nur mit Char als erstem Argument
    * ----------------------------------------------- */
   fputc(GET_CHAR(STACK(base, 0)), GET_CFILE(STACK(base, 1)));
}

/*-------------------------------------------------------------------------
 * C-ungetc char cfile
 *-------------------------------------------------------------------------*/
void 
c_ungetc(base)
CL_FORM *base;
{
   /* Aufruf erfolgt nur Char als erstem Argument
    * ------------------------------------------- */
   if (ungetc(GET_CHAR(STACK(base, 0)), GET_CFILE(STACK(base, 1))) == EOF)
      Lerror(STACK(base, 0), NOT_UNREAD);
}
