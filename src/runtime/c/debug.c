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
 * Function : primitive print functions which may be called by a debugger
 *            to inspect stack locations and other memeory locations
 *
 * $Revision: 1.26 $
 * $Id: debug.c,v 1.26 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/*----------------------------------------------------------------------------*/
int print_length = 10;
int print_level = 2;

/*------------------------------------------------------------------------------
 * Prozedur    : INSPECT
 * Beschreibung: Zeigt eine LISP-Form an
 *----------------------------------------------------------------------------*/
void inspect(base, level, length, in_list_p)
CL_FORM *base;
int level, length;
BOOL in_list_p;
{
   CL_FORM *fptr;
   long i, size;
   
   switch (TYPE_OF(base))
   {
	case CL_FIXNUM:              /*  1 */
      printf("%ld", GET_FIXNUM(base));
      return;
	case CL_FLOAT:               /*  2 */
      printf("%g[F]", GET_FLOAT(base));
      return;
	case CL_CHAR:                /*  3 */
      switch(GET_CHAR(base))
      {
		case '\b':
         printf("\\b");
         break;
		case '\f':
         printf("\\f");
         break;
		case '\n':
         printf("\\n");
         break;
		case '\r':
         printf("\\r");
         break;
		case '\t':
         printf("\\t");
         break;
		default:
         printf("\'%c\'", GET_CHAR(base));
      }
      return;
   case CL_UNBOUND:
      printf("#<unbound>");
      return;
	case CL_SYMBOL:              /* 13 */
      fptr = SYM_NAME(base);
      printf("%.*s", (int)AR_SIZE(fptr), CHAR_AR(fptr));
      return;
	case CL_NIL:                 /* 14 */
      printf("NIL");
      return;
	case CL_CLOSURE:             /* 32 */
      printf("#<Closure>");
      return;
	case CL_DOWNFUN:             /* 33 */
      printf("#<Downfun>");
      return;
	case CL_GLOBFUN:             /* 34 */
      printf("#<Global function>");
      return;
	case CL_IND:                 /* 38 */
      printf("Indirection: ");
      inspect(INDIRECT(base), level, length, FALSE);
      return;
	case CL_CFILE:               /* 41 */
      printf("#<C File>");
      return;
	case CL_UNIQUE_TAG:          /* 42 */
      printf("#<Tag %ld>", AR_SIZE(base));
      return;
   }

   if (level > print_level)     /* *PRINT-LEVEL* ueberschritten ? */
   {
      printf("#");
      return;
   }

   fptr = GET_FORM(base);
   switch (TYPE_OF(base))
   {
      /*----------------------------------------------------------------------*/
	case CL_CONS:                /* 15 */
      if (!in_list_p)
         printf("(");
      inspect(CAR(fptr), level + 1, 0, FALSE);
      fptr = CDR(fptr);
      switch (TYPE_OF(fptr))
      {
		case CL_NIL:
         printf(")");
         break;
		case CL_CONS:
         printf(" ");
         if (length > print_length)
         {
            printf("... )");
            return;
         }
         inspect(fptr, level, length + 1, TRUE);
         break;
		default:
         printf(" . ");
         inspect(fptr, level + 1, 0, FALSE);
         printf(")");
         break;
      }
      break;

      /*----------------------------------------------------------------------*/
	case CL_SMVEC_T:             /* 16 */
      size = AR_SIZE(fptr);
      printf("#1A%ld(", size);
      for (i = 0; i < size; i++)
      {
         inspect(OFFSET(AR_BASE(fptr), i), level + 1, 0, FALSE);
         if (i < size - 1)
            printf(" ");
         if (i > print_length)
         {
            printf("...");
            break;
         }
      }
      printf(")");
      break;

      /*----------------------------------------------------------------------*/
	case CL_SMVEC_FIXNUM:        /* 17 */
   {
      long *fixnum_ar = FIXNUM_AR(fptr);

      size = AR_SIZE(fptr);
      printf("#1AFi%ld(", size);
      for(i = 0; i < size; i++)
      {
         printf("%ld", fixnum_ar[i]);
         if (i < size - 1)
            printf(" ");
         if (i > print_length)
         {
            printf("...");
            break;
         }
      }
      printf(")");
      break;
   }

      /*----------------------------------------------------------------------*/
	case CL_SMVEC_FLOAT:         /* 18 */
   {
		double *fl_ptr = FLOAT_AR(fptr);

		size = AR_SIZE(fptr);
		printf("#1AFl%ld(", size);
		for (i = 0; i < size; i++, fl_ptr++)
		{
         printf("%g", *fl_ptr);
         if (i < size - 1)
            printf(" ");
         if (i > print_length)
         {
            printf("...");
            break;
         }
		}
		printf(")");
		break;
   }
      /*----------------------------------------------------------------------*/
	case CL_SMVEC_CHARACTER:     /* 19 */
      printf("\"%.*s\"", (int)AR_SIZE(fptr), AR_STRING(fptr));
      break;

   case CL_SMVEC_BIT:           /* 20 */
      printf("<Bitvector>");
      break; 
      
      /*----------------------------------------------------------------------*/
	case CL_STRUCT:              /* 39 */
   {
		size = 1 + AR_SIZE(fptr);
		printf("#S(");
		for (i = 0; i < size; i++)
		{
         inspect(fptr + 1 + i, level + 1, 0, FALSE);
         if (i < size - 1)
            printf(" ");
         if (i > print_length)
         {
            printf("...");
            break;
         }
		}
		printf(")");
		break;
   }

      /*----------------------------------------------------------------------*/
	case CL_INSTANCE:            /* 50 */
   {
		size = AR_SIZE(fptr);
		if (CL_NILP(AR_BASE(fptr)))
         printf("#<CLASS ");
		else
		{
         printf("#<INSTANCE-OF-");
         inspect(AR_BASE(fptr), level + 1, 0, FALSE);
         printf(" ");
		}
		for (i = 1; i < size; i++)
		{
         inspect(fptr + 1 + i, level + 1, 0, FALSE);
         if (i < size - 1)
            printf(" ");
         if (i > print_length)
         {
            printf("...");
            break;
         }
		}
		printf(">");
		break;
   }

      /*----------------------------------------------------------------------*/
#ifdef RT_CHAR_PTR
   case RT_CHAR_PTR:
      printf("<char_ptr>\"%s\"", GET_CHAR_PTR(base));
      break;
   case RT_FLOAT_PTR:
      printf("<float_ptr %lx>", (long)GET_FLOAT_PTR(base));
      break;
   case RT_FIXNUM_PTR:
      printf("<fixnum_ptr %lx>", (long)GET_FIXNUM_PTR(base));
      break;
   case RT_FORM_PTR:
      printf("<form_ptr %lx>", (long)GET_FORM(base));
      break;
#endif
	default:
      printf("<Unknown object %d>", TYPE_OF(base));
   }
}

/*------------------------------------------------------------------------------
 * Zeigt aus dem Speicherbereich ab 'base + offset' 'nargs' viele Eintraege an
 *----------------------------------------------------------------------------*/
void stack_cont (base, offset, nargs)
CL_FORM *base;
int offset, nargs;
{
   int i;

   extern CL_FORM *fo_heap;     /* aus system.c */
   extern unsigned form_heapsize;

   /* Argumente auf Plausibilität prüfen, um BUS-Errors einzuschränken
    * Die Zahlen sind so gewählt, daß der häufigste Fall, bei
    * stack_cont(&Smain[100],0,1) das & zu vergessen, wahrscheinlich 
    * abgefangen wird. */
   if (offset < 0 || nargs < 0 || offset > 10000 || nargs > 1000 ||
       (long)base < 100 || base > fo_heap + 2 * form_heapsize)
   {
      printf("\n*** Ungueltige Argumente\n");
      return;
   }

   for (i = offset; i < offset + nargs; i++)
   {
      printf ("%2d: ", i);
      inspect (base + i, 0, 0, FALSE);
      printf ("\n");
   }
}

/*------------------------------------------------------------------------------
 * Zeigt den Namen, die Property-Liste und den Wert des Symbols an einer
 * gegebenen Adresse
 *----------------------------------------------------------------------------*/
void dc_pure_symbol (symbol)
CL_FORM *symbol;
{
   printf ("Symbol (0x%lx): %.*s\n",
           (long)symbol,
           (int)AR_SIZE(symbol + OFF_SYM_NAME),
           AR_STRING(symbol + OFF_SYM_NAME));
   printf ("Plist: ");
   inspect (symbol + OFF_SYM_PLIST, 0, 0, FALSE);
   printf ("\nValue: ");
   inspect (symbol + OFF_SYM_VALUE, 0, 0, FALSE);
   printf ("\n");
}

/*------------------------------------------------------------------------------
 * Zugriff auf das i-te Symbol im Symbol-Array eines Moduls
 *----------------------------------------------------------------------------*/
void symbol_module_i (sym_base, i)
CL_FORM *sym_base;
int i;
{
   dc_pure_symbol(SYMBOL(sym_base, i));
}

/*------------------------------------------------------------------------------
 * Zeigt eine Alist formatiert an.
 *----------------------------------------------------------------------------*/
void show_alist (base, i)
CL_FORM *base;
int i;
{
   printf("(");
   while (!CL_NILP(base))
   {
      if (!CL_CONSP(base))
      {
         printf("<error, ");
         inspect(base, 0, 0, FALSE);
         printf(" instead of cons>");
         break;
      }
      inspect(GET_CAR(base), 0, 0, FALSE);
      printf("\n ");
      base = GET_CDR(base);
   }
   printf(")\n");
}
