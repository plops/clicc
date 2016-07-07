/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : Laufzeitsystem: Definition des C Hauptprogramms
 *
 * $Revision: 1.11 $
 * $Log: main.c,v $
 * Revision 1.11  1993/07/09  13:43:28  hk
 * In initialize: Variable noargs static deklariert.
 *
 * Revision 1.10  1993/06/17  10:32:14  hk
 * Aufruf von stack_cont vor das return verschoben, damit es nicht
 * wegoptimiert wird.
 *
 * Revision 1.9  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.8  1993/06/04  19:37:32  uho
 * save_stack hinter die Speicheranforderung verschoben.
 *
 * Revision 1.7  1993/06/04  14:12:43  uho
 * initialize fuer die Integration in C-Hauptprogramme eingefuehrt.
 *
 * Revision 1.6  1993/06/04  13:46:12  pm
 * initialisieren der globalen Vaiablen save_stack
 *
 * Revision 1.5  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.4  1993/02/17  15:40:24  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.3  1992/10/02  14:56:13  uho
 * memallot einegefuegt, um Heapspeicher dynamisch anzufordern.
 * stack_cont dem Prototypen nach mit dummy-Parametern versehen.
 * Extern Deklarationen durch Einlesen von fun_decl.h ersetzt.
 *
 * Revision 1.2  1992/09/23  14:44:59  hk
 * ILLEGAL_ARGS definiert. (fuer funcall und mv-lambda).
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *
 *               1992/01/20            hk
 * #ifdef DEBUG um stack_cont();
 *               1991/07/30            hk
 * argc und argv Parameter eingefuegt.
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

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

   startup(stack);
   
#ifdef DEBUG
   /* um eine Referenz darauf zu erzwingen */
   stack_cont(stack,0,0);
#endif

   return(GET_FIXNUM(stack));
}
