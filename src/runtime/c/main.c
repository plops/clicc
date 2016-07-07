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
 * Funktion : Definition des C Hauptprogramms
 *
 * $Revision: 1.15 $
 * $Id: main.c,v 1.15 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"


/* Referenz in das Lisp-Modul */
/*----------------------------*/
extern void rt_startup(/* CL_FORM *base */);

/* wird im erzeugten Code verwendet */
/*----------------------------------*/
char TOO_MANY_ARGS[] = "too many Arguments";
char ILLEGAL_ARGS[] =  "illegal number of arguments";

/* Zaehler fuer TAGs von automatisch generierten CATCHern */
/*--------------------------------------------------------*/
long tag_counter = 0;

BOOL bool_result = FALSE;
int glob_argc;
char **glob_argv;

#ifdef NOMAIN

int initialize() 
{
   static char *noargs[] =  { NULL };
   glob_argc = 0;
   glob_argv = noargs;

#else   

int main(argc, argv)
int argc;
char *argv[]; 
{
   glob_argc = argc;
   glob_argv = argv;

#endif

   memallot();

   save_stack = stack;

   rt_startup(stack);
   
#ifdef DEBUG
   /* um eine Referenz darauf zu erzwingen */
   stack_cont(stack,0,0);
#endif

   return(GET_FIXNUM(stack));
}
