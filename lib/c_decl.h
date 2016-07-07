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
 * Funktion : Deklarationen fuer das Laufzeitsystem
 * 
 * $Revision: 1.46 $
 * $Id: c_decl.h,v 1.46 1994/11/22 15:10:31 hk Exp $
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <setjmp.h>

/*------------------------------------------------------------------------------
 * Konfiguration
 *----------------------------------------------------------------------------*/
#include <obrep.h>

/* #define INT_GE_PTR */      /* sizeof(int) >= sizeof(void *) */

#ifdef __STDC__                 /* PTR ist allgemeiner Zeiger */
typedef void *PTR;
#else
typedef char *PTR;
#endif


/*------------------------------------------------------------------------------
 * Typdefinitionen
 *----------------------------------------------------------------------------*/
#define NOT(x)  (!(x)) /* zur besseren Lesbarkeit des erzeugten Codes */
#define BOOL  char
#ifndef FALSE
#define FALSE  0
#define TRUE  1
#endif


/*------------------------------------------------------------------------------
 * Vorwärts-Deklaration
 *----------------------------------------------------------------------------*/
typedef struct global_funarg GLOBAL_FUNARG;
typedef struct down_funarg DOWN_FUNARG;
typedef struct contenv CONTENV;


/*------------------------------------------------------------------------------
 * Typen der von CLICC erzeugten C-Funktionen
 *----------------------------------------------------------------------------*/
typedef void GLOBAL_FUN  (/*CL_FORM *base, ...*/);
typedef void LOCAL_FUN   (/*CL_FORM *base, CL_FORM **display, ...*/);
typedef void CLOSURE_FUN (/*CL_FORM *base, ...*/);


/*------------------------------------------------------------------------------
 * Alle Datenrepräsentationsabhängigen Definitionen einbinden...
 *----------------------------------------------------------------------------*/
#if OBREP == 1
#include "obrep1.h"
#elif OBREP == 2
#include "obrep2.h"
#else
/* Der Whitespace vor dem #error ist notwendig, damit Pre-ANSI-C-Compiler
 * das unbekannte Praeprozessorkommando ggf. ignorieren. */
	#error "OBREP is not defined or has an unsupported value"
#endif


/*------------------------------------------------------------------------------
 * Datenstruktur für Continuations
 *----------------------------------------------------------------------------*/
struct contenv
{
   struct contenv *last;
   CL_FORM *bind_top;
   jmp_buf jmp_buf;
};


/*------------------------------------------------------------------------------
 * Makro für den Zugriff auf den Stackframe einer LISP-Funktion
 *----------------------------------------------------------------------------*/
#define STACK(base, offset) ((base) + (offset))

/*------------------------------------------------------------------------------
 * Makro speziell für den Zugriff auf den LISP-Stack definiert durch "base"
 *----------------------------------------------------------------------------*/
#define ARG(offset) ((base) + (offset))

/*------------------------------------------------------------------------------
 * Makro für den Zugriff auf ein Element eines Vektors von CL_FORMs
 *----------------------------------------------------------------------------*/
#define OFFSET(vector, offset) ((vector) + (offset))


/*------------------------------------------------------------------------------
 * Makro für das Kopieren der CL_FORM-Struktur
 * Bei einigen Kompilern muss Komponentenweise kopiert werden.
 *----------------------------------------------------------------------------*/
#define COPY(source, dest) (*(dest) = *(source))

/* Falls komponentenweise kopiert werden muss, dann unbedingt den Typ
 * zuerst kopieren! Denn sonst wurden Aufrufe der Form
 * COPY(SYM_VALUE(ARG(1)), ARG(1))
 * nicht korrekt uebersetzt werden.
 *    #define COPY(source, dest)\
 *       (dest->tag = source->tag, dest->val = source->val)
 */


#ifdef __STDC__
#define LISP_FUN(name) void name(CL_FORM *base)
#define LISP_FUN_NARGS(name) void name(CL_FORM *base, int nargs)
#else
#define LISP_FUN(name) void name(base) CL_FORM *base;
#define LISP_FUN_NARGS(name) void name(base, nargs) CL_FORM *base; int nargs;
#endif


/*------------------------------------------------------------------------------
 * Retten/Restaurieren von SPECIAL Variablen.
 * Retten: 1. Wert auf Binding-Stack
 *         2. Symbol auf Binding-Stack.
 * Restaurieren: alten Wert in die Wert-Zelle des Symbols kopieren. 
 * 'sym' ist Zeiger auf das Symbol direkt, NICHT Zeiger auf eine CL_FORM
 * mit Typ CL_SYMBOL.
 *----------------------------------------------------------------------------*/
#define POP_SPECIAL bind_top -= 2
#define SAVE_SPECIAL(sym)  (COPY(OFF_SYM_VALUE + sym, bind_top), \
                            bind_top++, \
                            LOAD_SYMBOL(sym, bind_top), \
                            bind_top++)
#define BIND_SPECIAL(sym, new_val)  (SAVE_SPECIAL(sym), \
                                     COPY(new_val, OFF_SYM_VALUE + sym))
#define RESTORE_SPECIAL  (POP_SPECIAL, \
                          COPY(bind_top, SYM_VALUE(bind_top+1)))


/*-------------------------------------------------------------------------
 * Verschiebt in einem Array von CL_FORMs den Bereich [SrcLow .. SrcHigh[ um
 * MoveDiff CL_FORMs in Richtung hoeherer Adressen. Quelle und Ziel koennen
 * sich uebelappen.
 *-------------------------------------------------------------------------*/
#define MEM_UP_MOVE(SrcHigh, SrcLow, MoveDiff) \
{  CL_FORM *from = (SrcHigh),\
           *to   = from + (MoveDiff);\
      while(from > (SrcLow)) { \
          from--, to--; \
          COPY(from, to); \
      } \
} 


/*-------------------------------------------------------------------------
 * Kopiert N-1 CL_FORMs beginned bei Offset im aktuellen Activation Record in
 * den Multiple-Value Buffer.
 *-------------------------------------------------------------------------*/
#define MV_TO_STACK(n, offset) \
{ int i; \
  for (i=0; i<(n)-1; i++) { \
     COPY(&mv_buf[i], ARG(i+(offset)));\
  } \
}

/*------------------------------------------------------------------------------
 * Ein Befehl, der bei der ersten Ausfuehrung keine sichtbaren Effekt
 * hat und bei nachfolgenden Ausfuehrungen die Ausfuehrung der
 * aktuellen Funktion abbricht.
 *----------------------------------------------------------------------------*/
#define ONLY_ONCE \
{static BOOL was_here = FALSE; if(was_here) return; else was_here = TRUE;} 
  
/*------------------------------------------------------------------------------
 * eigenes setjmp/longjmp definieren, damit auch auf Maschinen, bei denen
 * sizeof(char*) > sizeof(int) gilt, Zeiger als Parameter von longjmp 
 * angegeben werden können.
 *----------------------------------------------------------------------------*/
#ifdef INT_GE_PTR
#define SETJMP(buf)  setjmp(buf)
#define LONGJMP(buf, value)  longjmp(buf, (int)(value))
#else
 extern char *jmp_value;
#define SETJMP(buf)  (setjmp(buf) ? jmp_value : NULL)
#define LONGJMP(buf, value)  (jmp_value = (char *)(value), longjmp(buf, 1))
#endif

/*------------------------------------------------------------------------------
 * Konvertiert C-bool in LISP-Äquivalent
 *----------------------------------------------------------------------------*/
#define LOAD_BOOL(expr, loc)  if (expr) LOAD_T(loc); else LOAD_NIL(loc)

#define RET_BOOL(expr)  LOAD_BOOL(expr, ARG(0))

#define RET_BOOL_OPT(expr)  if (!(expr)) LOAD_NIL(ARG(0))

/*------------------------------------------------------------------------------
 * Die Funktion get_c_string ist einfach genug, ein Makro sein zu koennen,
 * dies beschleunigt sx_hashstring und allgemein Hashtabellen.
 *----------------------------------------------------------------------------*/
#define get_c_string(lisp_string) AR_STRING(GET_FORM(lisp_string))

/*------------------------------------------------------------------------------
 * Speicherverwaltung: Konsistenzpruefung und Test auf Konstanten
 *----------------------------------------------------------------------------*/
#ifdef SHARED_LIBRARY

/* Keine Annahme, Konstanten wuerden in Adressen unterhalb des Heaps
   abgelegt sein. Jede Adress ausserhalb des Heaps wird als Adresse
   eines Konstanten Datums angesehen. */

/* ---------------------- Float Heap ---------------------------------------- */
#define FL_CONSTANTq(fptr)  ((fptr) < fl_heap1 || \
                             (fptr) >= fl_heap1 + 2 * fl_heapsize)
#define FL_OUT_OF_HEAPq(fptr) (0)
#define FL_WRONG_HEAPq(fptr) ((fptr) < old_fl_heap)

/* ---------------------- Form Heap  ---------------------------------------- */
#define FO_CONSTANTq(fptr) ((fptr) < (fo_heap) || \
                            (fptr) >= fo_heap + 2 * form_heapsize)
#define FO_OUT_OF_HEAPq(fptr) (0)
#define FO_WRONG_HEAPq(fptr) ((fptr) < old_form_heap)

/* --------------------- String/Fixnum Heap --------------------------------- */
#define AR_CONSTANTq(fptr,offs) \
       (AR_STRING((fptr)+(offs)) < (char *)fx_heap1 || \
        AR_STRING((fptr)+(offs)) >= (char *)(fx_heap1 + 2 * fx_heapsize))

#else /* ===================== !SHARED_LIBRARY ============================== */

/* Annahme, Konstanden werden in Adressen unterhalb des Heaps abgelegt. */

/* ---------------------- Float Heap ---------------------------------------- */
#define FL_CONSTANTq(fptr)  ((fptr) < fl_heap1) 
#define FL_OUT_OF_HEAPq(fptr) ((fptr) >= fl_heap1 + 2 * fl_heapsize)
#define FL_WRONG_HEAPq(fptr) \
       ((fptr) < old_fl_heap || (fptr) >= old_fl_heap + fl_heapsize)

/* ---------------------- Form Heap  ---------------------------------------- */
#define FO_CONSTANTq(fptr)  ((fptr) < (fo_heap))
#define FO_OUT_OF_HEAPq(fptr) ((fptr) >= fo_heap + 2 * form_heapsize)
#define FO_WRONG_HEAPq(fptr) \
       ((fptr) < old_form_heap || fptr >= old_form_heap + form_heapsize)

#define AR_CONSTANTq(fptr,offs) (AR_STRING((fptr)+(offs)) < (char *)fx_heap1) 
/* --------------------- String/Fixnum Heap --------------------------------- */

#endif /* SHARED_LIBRARY */


/*------------------------------------------------------------------------------
 * Extern Deklarationen
 *----------------------------------------------------------------------------*/
extern char TOO_MANY_ARGS[];    /* in main.c */
extern char ILLEGAL_ARGS[];     /* in main.c */
extern long tag_counter;        /* in main.c */
extern BOOL bool_result;        /* in main.c */
extern CL_FORM *stack;          /* in system.c */
extern CL_FORM *fo_heap;        /* in system.c */
extern unsigned form_heapsize;  /* in system.c */
extern int mv_count;            /* in values.c */
extern CL_FORM mv_buf[];        /* in values.c */
extern CL_FORM bind_stack[];    /* in catch.c */
extern CL_FORM *bind_top;       /* in catch.c */
extern CONTENV *last_cont;      /* in catch.c */
extern CL_FORM *save_stack;     /* in foreign.c */
extern char IllegalType[];      /* in foreign.c */
extern CL_INIT classes[];       /* in generated C Code */

/* Ende */
