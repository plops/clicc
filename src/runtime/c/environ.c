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
 * Funktion : Zugriff auf System Resourcen
 *            - Environment Variablen
 *            - Komandozeilen Argumente
 *
 * $Revision: 1.14 $
 * $Id: environ.c,v 1.14 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern int glob_argc;         /* in main.c */
extern char **glob_argv;      /* in main.c */

char ARG_FILE_NOT_FOUND[]= "File ~a not found for argument expansion";
char ARG_FILE_ERR[]= "in file ~a while expanding argument";
char ARG_STRING_TOO_LONG[]= "Argument string too long: ~a";

/*------------------------------------------------------------------------------
 * c-getenv-internal string
 *----------------------------------------------------------------------------*/
void c_getenv_internal(base)
CL_FORM *base;
{
   char *str = getenv(get_c_string(ARG(0)));

   if(str == NULL)
   {
      LOAD_NIL(ARG(0));
   }
   else
   {
      make_string(ARG(0), str);
   }
}

/*------------------------------------------------------------------------------
 * c-system_internal string
 *----------------------------------------------------------------------------*/
void c_system_internal(base)
CL_FORM *base;
{	
   LOAD_FIXNUM(ARG(0), system(get_c_string(ARG(0))), ARG(0));
}

/*------------------------------------------------------------------------------
 * c-argc
 *----------------------------------------------------------------------------*/
void c_argc(base)
CL_FORM *base;
{
   LOAD_FIXNUM(ARG(0), glob_argc, ARG(0));
}

/*------------------------------------------------------------------------------
 * c-argv
 *----------------------------------------------------------------------------*/
void c_argv(base)
CL_FORM *base;
{
   char tmp_buf[256];
   int arg = 0;
   int stk = 0;
   
   while(arg < glob_argc)
   {
      /* expand argument "@filename" to contents of file */
      /* !!! Dokumentieren ! */
      /* ----------------------------------------------- */
      if('@' == glob_argv[arg][0])
      {
         FILE *f;

         f = fopen(&glob_argv[arg][1], "r");
         if(!f)
         {
            make_string(ARG(stk), &glob_argv[arg][1]);
            Lerror(ARG(stk), ARG_FILE_NOT_FOUND);
         }
         else
         {
            int j;

            j = 0;
            do
            {
               tmp_buf[j] = getc(f);
               if(isspace(tmp_buf[j]) || EOF == tmp_buf[j])
               {
                  if(ferror (f))
                  {
                     make_string(ARG(stk), &glob_argv[arg][1]);
                     Lerror(ARG(stk), ARG_FILE_ERR);
                  }
                  if(j > 0)
                  {
                     tmp_buf[j] = 0;
                     make_string(ARG(stk), tmp_buf);
                     stk++;
                  }
                  j = 0;
               }
               else
               {
                  j++;
                  if(j >= sizeof(tmp_buf))
                  {
                     tmp_buf[--j]='\0';
                     make_string(ARG(stk), tmp_buf);
                     Lerror(ARG(stk), ARG_STRING_TOO_LONG);
                  }
               }                  
            }
            while(!feof(f));
            arg++;
         }
      }
      else
      {
         make_string(ARG(stk), glob_argv[arg]);
         stk++;
         arg++;
      }
   }
   Flist(ARG(0), stk);
   glob_argc = stk;
}
