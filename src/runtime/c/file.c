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
 * Funktion : - c_fopen
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
 * $Revision: 1.12 $
 * $Id: file.c,v 1.12 1994/11/22 14:54:01 hk Exp $
 *--------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"


char NOT_UNREAD[] = "Could not unread character ~s";

/*------------------------------------------------------------------------------
 * C-OPEN name mode
 *----------------------------------------------------------------------------*/
LISP_FUN(c_fopen)
{
   FILE *f = fopen(get_c_string(ARG(0)), get_c_string(ARG(1)));

   if (f != NULL)
   {
      LOAD_CFILE(ARG(0), f, ARG(0));
   }
   else
      LOAD_NIL(ARG(0));
}

/*------------------------------------------------------------------------------
 * C-FCLOSE cfile
 *----------------------------------------------------------------------------*/
LISP_FUN(c_fclose)
{
   fclose(GET_CFILE(ARG(0)));
}

/*------------------------------------------------------------------------------
 * C-FTELL cfile
 *----------------------------------------------------------------------------*/
LISP_FUN(c_ftell)
{
   LOAD_FIXNUM(ARG(1), ftell(GET_CFILE(ARG(0))), ARG(0));
}

/*------------------------------------------------------------------------------
 * C-FSEEK cfile offset
 *----------------------------------------------------------------------------*/
LISP_FUN(c_fseek)
{
   fseek(GET_CFILE(ARG(0)), (long)GET_FIXNUM(ARG(1)), 0);
}

/*------------------------------------------------------------------------------
 * C-FILE-LENGTH cfile
 *----------------------------------------------------------------------------*/
LISP_FUN(c_file_length)
{
   FILE *f = GET_CFILE(ARG(0));
   long pos = ftell(f);
   long len;

   /* Dateiende */
   fseek(f, 0L, 2);
   len = ftell(f);
   fseek(f, pos, 0);
   LOAD_FIXNUM(ARG(0), len, ARG(0));
}

/*------------------------------------------------------------------------------
 * C-STDIN
 *----------------------------------------------------------------------------*/
LISP_FUN(c_stdin)
{
   LOAD_CFILE(ARG(0), stdin, ARG(0));
}

/*------------------------------------------------------------------------------
 * C-STDOUT
 *----------------------------------------------------------------------------*/
LISP_FUN(c_stdout)
{
   LOAD_CFILE(ARG(0), stdout, ARG(0));
}

/*------------------------------------------------------------------------------
 * C-FGETC cfile
 *----------------------------------------------------------------------------*/
#if 1
LISP_FUN(c_fgetc)
{
   int c = fgetc(GET_CFILE(ARG(0)));

   if (c == EOF)
      LOAD_NIL(ARG(0));
   else
   {
      LOAD_CHAR(ARG(0), c, ARG(0));
   }
}
#else
LISP_FUN(c_fgetc)
{
   static char buf[256];
   static char *ptr = buf;

   if (*ptr == '\0')
   {
      if (!fgets(buf, sizeof(buf), GET_CFILE(ARG(0))))
      {
         LOAD_NIL(ARG(0));
         buf[0] = '\0';
         ptr = buf;
         return;
      }
      else
      {
         ptr = buf;
      }
   }
   LOAD_CHAR(ARG(0), *ptr++, ARG(0));
}
#endif

/*------------------------------------------------------------------------------
 * C-FPUTC char cfile
 *----------------------------------------------------------------------------*/
LISP_FUN(c_fputc)
{
   fputc((int)GET_CHAR(ARG(0)), GET_CFILE(ARG(1)));
}

/*-------------------------------------------------------------------------
 * C-UNGETC char cfile
 *-------------------------------------------------------------------------*/
LISP_FUN(c_ungetc)
{
   if (ungetc((int)GET_CHAR(ARG(0)), GET_CFILE(ARG(1))) == EOF)
      Lerror(ARG(0), NOT_UNREAD);
}
