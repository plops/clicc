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
 * Funktion : Speicherverwaltung, Abort-Funktion
 *
 * $Revision: 1.29 $
 * $Id: system.c,v 1.29 1994/11/29 15:09:43 hk Exp $
 *----------------------------------------------------------------------------*/
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <c_decl.h>
#include <string.h>
#include "sys.h"

/*------------------------------------------------------------------------------
 * The sizes of the four memory areas used by the runtime system  will be
 * defined in the generated program as initialized global variables.
 *----------------------------------------------------------------------------*/

extern unsigned maxstacksize;
extern unsigned minstacksize;

extern unsigned maxform_heapsize;
extern unsigned minform_heapsize;

extern unsigned maxfx_heapsize;
extern unsigned minfx_heapsize;

extern unsigned maxfl_heapsize;
extern unsigned minfl_heapsize;

/*------------------------------------------------------------------------------
 * LISP-Laufzeitstack
 *----------------------------------------------------------------------------*/
CL_FORM *stack;
CL_FORM *eos;              /* end of stack */
unsigned stacksize;

/*------------------------------------------------------------------------------
 * LISP-Heap fuer CL_FORMs
 *----------------------------------------------------------------------------*/
CL_FORM *fo_heap;
CL_FORM *form_heap;
CL_FORM *form_toh;
CL_FORM *form_eoh;
CL_FORM *old_form_heap;
unsigned form_heapsize;

/*------------------------------------------------------------------------------
 * LISP-Heap fuer INTEGERs und STRINGs
 *----------------------------------------------------------------------------*/
long *fx_heap1;

long *fx_heap;
long *fx_toh;
long *fx_eoh;

long *old_fx_heap;

unsigned fx_heapsize;


/*------------------------------------------------------------------------------
 * LISP-Heap fuer FLOATs
 *----------------------------------------------------------------------------*/
double *fl_heap1;

double *fl_heap;
double *fl_toh;
double *fl_eoh;

double *old_fl_heap;

unsigned fl_heapsize;

/*------------------------------------------------------------------------------
 * PTR allocate(nelem, minelem, elsize, actelem)
 * unsigned nelem, minelem, elsize, *actnelem;
 *
 * Try to allocate a block of memory containing elements of size ELSIZE
 * Allocates between MINELEM and NELEM elements.
 * Returns a pointer to the newly allocated block, or NULL if not succesful
 * ACTELEM is assigne to the number of elements actually allocated.
 *----------------------------------------------------------------------------*/
#if __STDC__
static PTR allocate(unsigned, unsigned, unsigned, unsigned *);
#endif
static PTR allocate(nelem, minelem, elsize, actnelem)
unsigned nelem, minelem, elsize, *actnelem;
{
   PTR heap;

   while ((heap = malloc(nelem * elsize)) == NULL)
   {
      nelem /= 2;
      if (nelem < minelem)
      {  /* less than required, try minimal requirements */
         if ((heap = malloc(minelem * elsize)) == NULL)
         {
            *actnelem = 0;
            return NULL;
         }
         *actnelem = minelem;
         return heap;
      }
   }
   *actnelem = nelem;
   return heap;
}


/*------------------------------------------------------------------------------
 * Fehlermeldung
 *----------------------------------------------------------------------------*/
#if __STDC__
  static void no_mem(char *msg, unsigned amount);
#endif
static void no_mem(msg, amount)
char *msg;
unsigned amount;
{
   fprintf(stderr, "Cannot allocate %u Bytes for %s!\n", amount, msg);
   Labort("Out of Memory!");
}


/*------------------------------------------------------------------------------
 * void memallot()
 *
 *  Alloctes memory for the different regions needed by the runtime
 *  system.
 *  Assigns the global veriables
 *
 *  Stack     : stack, stacksize, eos,
 *  FormHeap  : fo_heap, form_heapsize, form_toh, form_heap, form_eoh,
 *  IntStrHeap: fx_heap1, fx_heapsize, fx_heap, fx_toh, fx_eoh, old_fx_heap,
 *  FloatHep  : fl_heap1, fl_heapsize, fl_heap, fl_toh, fl_eoh, old_fl_heap
 *
 *  If not enough memory can be allocated, MEMALLOT will raise an error and
 *  will abort program execution.
 *
 *----------------------------------------------------------------------------*/
void memallot()
{
   /* allocate stack */
   /*----------------*/
   stack = (CL_FORM *)allocate(maxstacksize, minstacksize,
                               sizeof(CL_FORM), &stacksize);
   if(stack == NULL)
      no_mem("Stack", minstacksize*sizeof(CL_FORM));
   eos = stack + stacksize;

   /* allocate form heap */
   /*--------------------*/
   fo_heap = (CL_FORM *)allocate(2 * maxform_heapsize, 2 * minform_heapsize,
                                 sizeof(CL_FORM), &form_heapsize);
   if(fo_heap == NULL) 
      no_mem("FormHeap", 2*minform_heapsize*sizeof(CL_FORM));

   form_heapsize /= 2;
   form_heap = form_toh = fo_heap;
   form_eoh = old_form_heap = fo_heap + form_heapsize;

   /* allocate integer- and string heap */
   /*-----------------------------------*/
   fx_heap1 = (long *)allocate(2 * maxfx_heapsize, 2 * minfx_heapsize,
                               sizeof(long), &fx_heapsize);
   if(fx_heap1 == NULL) 
      no_mem("Integer/StringHeap", 2*minfx_heapsize*sizeof(long));

   fx_heapsize /= 2;
   fx_heap = fx_toh = fx_heap1;
   fx_eoh = old_fx_heap = fx_heap1 + fx_heapsize;

   /* allocate float heap */
   /*---------------------*/
   fl_heap1 = (double *)allocate(2 * maxfl_heapsize, 2 * minfl_heapsize,
                                 sizeof(double), &fl_heapsize);
   if(fl_heap1 == NULL)
      no_mem("FloatHeap", 2*minfl_heapsize*sizeof(double));

   fl_heapsize /= 2;
   fl_heap = fl_toh = fl_heap1;
   fl_eoh = old_fl_heap = fl_heap1 + fl_heapsize;

   INIT_FUN;
}


/*------------------------------------------------------------------------------
 * Kopiert 'num' FORMs in den neuen Heap.
 *----------------------------------------------------------------------------*/
CL_FORM *form_swap (from, num)
CL_FORM *from;
long num;
{
   CL_FORM *new_location = form_toh;

   memcpy((PTR)form_toh, (PTR)from, num * sizeof(CL_FORM));
   form_toh += num;
   return new_location;
}

/*------------------------------------------------------------------------------
 * Kopiert 'num' FIXNUMs in den neuen Heap.
 *----------------------------------------------------------------------------*/
long *fx_swap (from, num)
long *from;
long num;
{
   long *new_location = fx_toh;

   memcpy((PTR)fx_toh, (PTR)from, num * sizeof(long));
   fx_toh += num;
   return new_location;
}

/*------------------------------------------------------------------------------
 * Kopiert 'num' + 1 CHARACTERs in den neuen Heap.
 * (einschliesslich des abschliessenden 0 Characters)
 *----------------------------------------------------------------------------*/
char *ch_swap (from, num)
char *from;
long num;
{
   char *new_location = (char *)fx_toh;

   /* Auf nächstens Vielfaches von sizeof(long) runden */
   /* ANNAHME: sizeof(long) ist 2er Potenz! */
   num = ((num + sizeof(long)-1 + 1) & ~(sizeof(long)-1)) / sizeof(long);

   memcpy((PTR)fx_toh, from, num * sizeof(long));
   fx_toh += num;
   return new_location;
}

/*------------------------------------------------------------------------------
 * Kopiert 'num' FLOATs in den neuen Heap.
 *----------------------------------------------------------------------------*/
double *fl_swap (from, num)
double *from;
long num;
{
   double *new_location = fl_toh;

   /* ACHTUNG: wenn mehrere Referenzen auf eine Fließkommazahl existieren, */
   /* dann wird die Zahl mehrmals kopiert ! --> evtl. Überlauf während */
   /* der GC */
   /*--------*/
   if(fl_toh + num >= fl_eoh)
      Labort("Float-Heap Overflow.");

   memcpy((PTR)fl_toh, (PTR)from, num * sizeof(double));
   fl_toh += num;
   return new_location;
}

/*------------------------------------------------------------------------------
 * Kopiert 'num' BITS eines Bitvektors in den neuen Heap.
 *----------------------------------------------------------------------------*/
long *bits_swap (from, num)
long *from;
long num;
{
   long *new_location = fx_toh;

   num = (num + BITS_PER_FIXNUM - 1) / BITS_PER_FIXNUM;
   memcpy((PTR)fx_toh, (PTR)from, num * sizeof(long));
   fx_toh += num;
   return new_location;
}

/*------------------------------------------------------------------------------
 * Garbage-Collector
 *----------------------------------------------------------------------------*/
void gc (base, type)
CL_FORM *base;
char type; 
{
   extern void gc_main();
   CL_FORM *form_tmp = form_heap;
   long    *fx_tmp = fx_heap;
   double  *fl_tmp = fl_heap;
   double fo_room, fx_room, fl_room;

#ifdef DEBUG
   fprintf (stderr, ";;; GC -%c- ", type);
   fflush(stderr);
#endif

   /* Vertauschen der Heap-Bereiche */
   /* ----------------------------- */
   form_heap = old_form_heap;
   old_form_heap = form_tmp;
   form_toh = form_heap;
   form_eoh = form_heap + form_heapsize;

   fx_heap = old_fx_heap;
   old_fx_heap = fx_tmp;
   fx_toh = fx_heap;
   fx_eoh = fx_heap + fx_heapsize;

   fl_heap = old_fl_heap;
   old_fl_heap = fl_tmp;
   fl_toh = fl_heap;
   fl_eoh = fl_heap + fl_heapsize;

   do_gc(base);

#ifdef DEBUG
   fo_room = (double)(form_toh - form_heap) / form_heapsize;
   fx_room = (double)(fx_toh - fx_heap) / fx_heapsize;
   fl_room = (double)(fl_toh - fl_heap) / fl_heapsize;
   fprintf(stderr, "Allocated: %5.2f%% Forms, %5.2f%% \
Characters/Fixnums, %5.2f%% Floats \n",
           100 * fo_room, 100 * fx_room, 100 * fl_room);
#endif
}

/*------------------------------------------------------------------------------
 * Traversiert die Symbole eines Moduls
 *----------------------------------------------------------------------------*/
void gc_symbols (symbol)
CL_INIT *symbol;
{
   while (!IS_END_SYMDEF(symbol))
   {
      SAVE_FORM ((CL_FORM *)OFFSET(symbol, OFF_SYM_PLIST));
      SAVE_FORM ((CL_FORM *)OFFSET(symbol, OFF_SYM_VALUE));
      SAVE_FORM ((CL_FORM *)OFFSET(symbol, OFF_SYM_PACKAGE));
      symbol += SYM_SIZE;
   }
}

/*------------------------------------------------------------------------------
 * Alloziert Speicherplatz fuer 'num' Integers im Heap.
 * Annahme: Integer benoetigen gerade Anzahl von Bytes.
 * Rueckgabewert: Zeiger -> Beginn des allozierten Speicherbereichs
 *----------------------------------------------------------------------------*/
long *fixnum_alloc (base, num)
CL_FORM *base;
long num;
{
   long *ptr;

   /* Falls der Heap voll ist, Garbage-Collector starten */
   /* -------------------------------------------------- */
   if (fx_toh + num > fx_eoh)
   {
      gc (base, 'x');
      if (fx_toh + num > fx_eoh)
      {
         Labort("Not enough memory for heap allocation of FIXNUM");
      }
   }

   ptr = fx_toh;
   fx_toh += num;   /* Neuen TOH setzen */
   return ptr;
}

/*------------------------------------------------------------------------------
 * Alloziert Speicherplatz fuer 'num' + 1 Characters im Heap.
 * Jeder Lisp-String wird mit einem 0 Character abgeschlossen, damit eine
 * problemlose Uebertragung nach C moeglich ist.
 * Rueckgabewert: Zeiger auf Beginn des allozierten Speicherbereichs
 *----------------------------------------------------------------------------*/
char *char_alloc (base, rnum)
CL_FORM *base;
unsigned long rnum;
{
   char *ptr; 
   long num;

   /* Anzahl der LONG's (+ 1 fuer abschliessendes \0)*/
   num = (rnum + sizeof(long)) / sizeof(long);

   /* Falls der Heap voll ist, Garbage-Collector starten */
   /*----------------------------------------------------*/
   if (fx_toh + num > fx_eoh)
   {
      gc (base, 'c');
      if (fx_toh + num > fx_eoh)
      {
         Labort("Not enough memory for heap allocation of CHARACTER");
      }
   }

   ptr = (char *)fx_toh;
   ptr[rnum] = '\0';            /* Abschliessende 0 setzen */
   fx_toh += num;               /* Neuen Top of Heap setzen */
   return ptr;
}

/*------------------------------------------------------------------------------
 * Alloziert Speicherplatz fuer <num> Floats im Heap.
 * Rueckgabewert: Zeiger -> Beginn des allozierten Speicherbereichs
 *----------------------------------------------------------------------------*/
double *float_alloc (base, num)
CL_FORM *base;
long num;
{
   double *ptr;

   /* Falls der Heap voll ist, Garbage-Collector starten */
   /* -------------------------------------------------- */
   if (fl_toh + num > fl_eoh)
   {
      gc (base, 'l');
      if (fl_toh + num > fl_eoh)
      {
         Labort("Not enough memory for heap allocation of FLOAT");
      }
   }

   ptr = fl_toh;
   fl_toh += num;           /* Top of Heap erhoehen */
   return ptr;             /* Zeiger auf erste allozierte FLOAT zurueckgeben */
}

/*------------------------------------------------------------------------------
 * Alloziert Speicherplatz fuer <num> CL_FORM's im Heap.
 * Rueckgabewert: Zeiger -> Beginn des allozierten Speicherbereichs
 *----------------------------------------------------------------------------*/
CL_FORM *form_alloc (base, num)
CL_FORM *base;
long num;
{
   CL_FORM *lptr;

    /* Kein Platz mehr fuer <num> Forms, Garbage Collector starten */
    /* ----------------------------------------------------------- */
   if (form_toh + num > form_eoh)
   {
      gc (base, 'o');
      if(form_toh + num > form_eoh)
      {
         Labort("Not enough memory for heap allocation of FORM");
      }
   }

   lptr = form_toh;
   form_toh += num;                   /* Top of Heap (Form) neu setzen */
   return lptr;                       /* Zeiger auf erste Form zurueckgeben */
}

/*------------------------------------------------------------------------------
 * Alloziert einen Bitvektor von <num> Bits in Form von Fixnums auf dem 
 * Heap. Die Konstante BITS_PER_FIXNUM definiert, wieviele Bits einer Fixnum
 * benutzt werden.
 *----------------------------------------------------------------------------*/
long *bits_alloc(base, num)
CL_FORM *base;
long num;
{
   long *ptr;
   num = (num + BITS_PER_FIXNUM - 1) / BITS_PER_FIXNUM;

   if (fx_toh + num > fx_eoh)
   {
      gc(base, 'b');
      if (fx_toh + num > fx_eoh)
      {
         Labort("Not enough memory for heap allocation of FIXNUM");
      }
   }

   ptr = fx_toh;
   fx_toh += num;
   return ptr;
}

/*------------------------------------------------------------------------------
 * Gibt eine Fehlermeldung aus und bricht die Programmausfuehrung ab.
 *----------------------------------------------------------------------------*/
void Labort (msg)
char *msg;
{
   fprintf(stderr, "\n*** Abort: %s\n", msg);
   exit(1);
}

/*------------------------------------------------------------------------------
 * Damit der C-Typinferenzer zufieden ist.
 *----------------------------------------------------------------------------*/
long FLAbort (msg)
char *msg;
{
   Labort(msg);
   return 0; /* dummy */
}

   
/*------------------------------------------------------------------------------
 * Gibt eine Laufzeitfehlermeldung aus und bricht ab.
 * Parameter: base ist einer Zeiger in den Lisp-Stack, an die Position des
 *                 fehlerhaften Ausdrucks
 *            msg ist ein Format-String fuer die Lisp-Funktion format
 *                als C-string
 *----------------------------------------------------------------------------*/
void Lerror (base, msg)
CL_FORM *base;
char *msg;
{
   extern void Ferror();
   make_string(ARG(1), msg);
   COPY(ARG(0), ARG(2));
   Ferror(ARG(1), 2);
}

/*------------------------------------------------------------------------------
 * Gibt eine Warnung zur Laufzeit aus.
 *----------------------------------------------------------------------------*/
void warning (warning_str)
char *warning_str;
{
   fprintf(stderr, ";;; Warning: %s\n", warning_str);
}
