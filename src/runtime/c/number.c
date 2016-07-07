/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : System-Funktionen: Numbers
 *
 * $Revision: 1.14 $
 * $Log: number.c,v $
 * Revision 1.14  1993/10/13  17:05:02  sma
 * Da GET_FIXNUM nicht immer als lvalue einsetzbar ist, ist
 * GE_TFIXNUM(STACK(base, 0))++ nicht allgemein möglich.
 *
 * Revision 1.13  1993/08/27  12:05:11  sma
 * "return 0" eingefuegt, um lint zufrieden zu stellen.
 *
 * Revision 1.12  1993/08/26  15:56:25  hk
 * Kommentare verschönert.
 *
 * Revision 1.11  1993/07/09  13:37:24  hk
 * In Finteger_length: (signed long) -> (long), wg. K&R-C.
 *
 * Revision 1.10  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.9  1993/06/07  08:05:23  sma
 * Schleife in Fnumeql kann mit 1 beginnen
 *
 * Revision 1.8  1993/04/29  10:31:05  ft
 * Erweiterung um asin, acos und atan.
 *
 * Revision 1.7  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.6  1993/03/02  15:27:28  hk
 * Schreibfehler in Finteger_length behoben.
 *
 * Revision 1.5  1993/02/17  15:41:03  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.4  1993/01/05  12:49:11  hk
 * Neue Funktion integer-length.
 *
 * Revision 1.3  1992/11/30  11:52:20  uho
 * Fehler in convert_to_int im ROUND-Fall beseitigt.
 * (Zuweisung im Praedikat des if-Konstrukts)
 *
 * Revision 1.2  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *               1991/12/16            hk
 * round-internal ohne rint definiert, zusaetzlich beruecksichtigt, dass
 * bei x.5 zur naechsten geraden Zahl gerundet wird.
 * ceiling-internal definiert
 *----------------------------------------------------------------------------*/

#include <math.h>
#include <c_decl.h>
#include "sys.h"


enum number_type {type_fixnum, type_float};

char No_number[]        = "~a is not a number";
char No_integer[]       = "~a is not an integer";
char Illegal_base_num[] = "Illegal base-number ~a with non-integer power-number 0";

/*------------------------------------------------------------------------------
 * Liefert ein LISP-Argument als C-FLOAT
 *----------------------------------------------------------------------------*/
double get_float(base)
CL_FORM *base;
{
   if (CL_FIXNUMP(STACK(base, 0)))
   	return((double)GET_FIXNUM(STACK(base, 0)));
   else if (CL_FLOATP(STACK(base, 0)))
   	return(GET_FLOAT(STACK(base, 0)));
   else
      Lerror(STACK(base, 0), No_number);
   return 0; /* damit lint zufrieden ist */
}

/*------------------------------------------------------------------------------
 * Alloziert die Floating-point Zahl 'num_float' im Heap.
 * 'base' zeigt auf den ersten freien Eintrag im Stack (GC).
 * Rueckgabewert: Zeiger auf die neu allozierte Zahl
 *----------------------------------------------------------------------------*/
double *make_float(base, num_float)
CL_FORM *base;
double   num_float;
{
   double *heap_float = float_alloc(base, 1L);
   *heap_float = num_float;
   return heap_float;
}

/*------------------------------------------------------------------------------
 * 12.2. Predicates on Numbers
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * zerop number
 *----------------------------------------------------------------------------*/
void Fzerop(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	RET_BOOL(GET_FIXNUM(STACK(base, 0)) == 0);
      break;
   case CL_FLOAT:
   	RET_BOOL(GET_FLOAT(STACK(base, 0)) == 0);
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
}

/*------------------------------------------------------------------------------
 * plusp number
 *----------------------------------------------------------------------------*/
void Fplusp(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	RET_BOOL(GET_FIXNUM(STACK(base, 0)) > 0);
      break;
   case CL_FLOAT:
   	RET_BOOL(GET_FLOAT(STACK(base, 0)) > 0);
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
}

/*------------------------------------------------------------------------------
 * minusp number
 *----------------------------------------------------------------------------*/
void Fminusp(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	RET_BOOL(GET_FIXNUM(STACK(base, 0)) < 0);
      break;
   case CL_FLOAT:
   	RET_BOOL(GET_FLOAT(STACK(base, 0)) < 0);
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
}

/*------------------------------------------------------------------------------
 * oddp integer
 *----------------------------------------------------------------------------*/
void Foddp(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	RET_BOOL(GET_FIXNUM(STACK(base, 0)) & 1);
      break;
   default: Lerror(STACK(base, 0), No_integer);
   }
}

/*------------------------------------------------------------------------------
 * evenp integer
 *----------------------------------------------------------------------------*/
void Fevenp(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	RET_BOOL(!(GET_FIXNUM(STACK(base, 0)) & 1));
      break;
   default: Lerror(STACK(base, 0), No_integer);
   }
}

/*------------------------------------------------------------------------------
 * Comparisons on Numbers
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * = number &REST more-numbers
 *----------------------------------------------------------------------------*/
void Fnumeql(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;

   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   {
   	long first = GET_FIXNUM(STACK(base, 0));

   	for(i = 1; i < nargs; i++)
      {
         switch(TYPE_OF(STACK(base, i)))
         {
         case CL_FIXNUM:
            if(GET_FIXNUM(STACK(base, i)) != first)
               goto NIL;
            break;
         case CL_FLOAT:
            if(GET_FLOAT(STACK(base, i)) != first)
               goto NIL;
            break;
         default: Lerror(STACK(base, i), No_number);
         }
      }
      goto T;
   }
   case CL_FLOAT:
   {
   	double first = GET_FLOAT(STACK(base, 0));

   	for(i = 1; i < nargs; i++)
      {
         switch(TYPE_OF(STACK(base, i)))
         {
         case CL_FIXNUM:
            if(GET_FIXNUM(STACK(base, i)) != first)
               goto NIL;
            break;
         case CL_FLOAT:
            if(GET_FLOAT(STACK(base, i)) != first)
               goto NIL;
            break;
         default: Lerror(STACK(base, i), No_number);
         }
      }
      goto T;
   }
   default: Lerror(STACK(base, 0), No_number);
   }
 T:
	LOAD_T(STACK(base, 0));
	return;
 NIL:
	LOAD_NIL(STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * /= number &REST more-numbers
 *----------------------------------------------------------------------------*/
void Fnumneql(base, nargs)
CL_FORM *base;
int nargs;
{
   int i, j;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */

   for(i = 0; i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      {
      	num_fixnum = GET_FIXNUM(STACK(base, i));
      	for(j = i + 1; j < nargs; j++)
         {
            switch(TYPE_OF(STACK(base, j)))
            {
            case CL_FIXNUM:
               if(GET_FIXNUM(STACK(base, j)) == num_fixnum)
                  goto NIL;
               break;
            case CL_FLOAT:
               if(GET_FLOAT(STACK(base, j)) == num_fixnum)
                  goto NIL;
               break;
            default: Lerror(STACK(base, j), No_number);
            }
         }
         break;
      }
      case CL_FLOAT:
      {
      	num_float = GET_FIXNUM(STACK(base, i));
      	for(j = i + 1; j < nargs; j++)
         {
            switch(TYPE_OF(STACK(base, j)))
            {
            case CL_FIXNUM:
               if(GET_FIXNUM(STACK(base, j)) == num_float)
                  goto NIL;
               break;
            case CL_FLOAT:
               if(GET_FLOAT(STACK(base, j)) == num_float)
                  goto NIL;
               break;
            default: Lerror(STACK(base, j), No_number);
            }
         }
         break;
      }
      default: Lerror(STACK(base, 0), No_number);
      }
   }
   LOAD_T(STACK(base, 0));
   return;

 NIL:
	LOAD_NIL(STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * < number &REST more-numbers
 *----------------------------------------------------------------------------*/
void Flt(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(STACK(base, 0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(STACK(base, 0));
   	num_type = type_float;
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum < GET_FIXNUM(STACK(base, i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float < GET_FIXNUM(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(STACK(base, i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum < GET_FLOAT(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float < GET_FLOAT(STACK(base, i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(STACK(base, i));
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   LOAD_T(STACK(base, 0));
   return;

 NIL:
	LOAD_NIL(STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * > number &REST more-numbers
 *----------------------------------------------------------------------------*/
void Fgt(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(STACK(base, 0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(STACK(base, 0));
   	num_type = type_float;
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum > GET_FIXNUM(STACK(base, i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float > GET_FIXNUM(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(STACK(base, i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum > GET_FLOAT(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float > GET_FLOAT(STACK(base, i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(STACK(base, i));
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   LOAD_T(STACK(base, 0));
   return;

 NIL:
	LOAD_NIL(STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * <= number &REST more-numbers
 *----------------------------------------------------------------------------*/
void Fle(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(STACK(base, 0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(STACK(base, 0));
   	num_type = type_float;
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum <= GET_FIXNUM(STACK(base, i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float <= GET_FIXNUM(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(STACK(base, i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum <= GET_FLOAT(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float <= GET_FLOAT(STACK(base, i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(STACK(base, i));
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   LOAD_T(STACK(base, 0));
   return;

 NIL:
	LOAD_NIL(STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * >= number &REST more-numbers
 *----------------------------------------------------------------------------*/
void Fge(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum;           /* LISP OBJECT of type FIXNUM */
   double num_float;            /* LISP OBJECT of type FLOAT  */
   enum number_type num_type;
   
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	num_fixnum = GET_FIXNUM(STACK(base, 0));
   	num_type = type_fixnum;
      break;
   case CL_FLOAT:
   	num_float = GET_FLOAT(STACK(base, 0));
   	num_type = type_float;
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
   for(i = 1; i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum >= GET_FIXNUM(STACK(base, i))))
      			goto NIL;
         }
      	else
         {
      		if(!(num_float >= GET_FIXNUM(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_fixnum;
         }
			num_fixnum = GET_FIXNUM(STACK(base, i));
         break;
      case CL_FLOAT:
      	if(num_type == type_fixnum)
         {
      		if(!(num_fixnum >= GET_FLOAT(STACK(base, i))))
      			goto NIL;
      		else
      			num_type = type_float;
         }
      	else
         {
      		if(!(num_float >= GET_FLOAT(STACK(base, i))))
      			goto NIL;
         }
			num_float = GET_FLOAT(STACK(base, i));
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   LOAD_T(STACK(base, 0));
   return;

 NIL:
	LOAD_NIL(STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * Arithmetic Operations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * + &rest numbers
 *----------------------------------------------------------------------------*/
void Fplus(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum = 0;       /* LISP OBJECT of type FIXNUM */
   double num_float  = 0;       /* LISP OBJECT of type FLOAT  */
   BOOL   float_res  = FALSE;   /* Kommt in 'numbers' eine FLOAT vor ? */
 
   for(i = 0; i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	num_fixnum += GET_FIXNUM(STACK(base, i));
         break;
      case CL_FLOAT:
      	num_float += GET_FLOAT(STACK(base, i));
      	float_res = TRUE;
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   if(float_res)
	{
      num_float += num_fixnum;
      LOAD_FLOAT(make_float(STACK(base, 0), num_float), STACK(base, 0));
	}
   else
      LOAD_FIXNUM(num_fixnum, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * - &rest numbers
 *----------------------------------------------------------------------------*/
void Fminus(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum = 0;       /* LISP OBJECT of type FIXNUM */
   double num_float  = 0;       /* LISP OBJECT of type FLOAT  */
   BOOL   float_res = FALSE;    /* Ist das Ergebnis eine FLOAT ? */
 
   if(nargs > 1)
	{
      switch(TYPE_OF(STACK(base, 0)))
      {
      case CL_FIXNUM:
      	num_fixnum = GET_FIXNUM(STACK(base, 0));
         break;
      case CL_FLOAT:
      	num_float = GET_FLOAT(STACK(base, 0));
      	float_res = TRUE;
         break;
      default: Lerror(STACK(base, 0), No_number);
      }
   }
   for(i =(nargs > 1 ? 1 : 0); i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	num_fixnum -= GET_FIXNUM(STACK(base, i));
         break;
      case CL_FLOAT:
      	num_float -= GET_FLOAT(STACK(base, i));
      	float_res = TRUE;
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   if(float_res)
	{
      num_float += num_fixnum;
      LOAD_FLOAT(make_float(STACK(base, 0), num_float), STACK(base, 0));
	}
   else
      LOAD_FIXNUM(num_fixnum, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * * &rest numbers
 *----------------------------------------------------------------------------*/
void Fmult(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum = 1;       /* LISP OBJECT of type FIXNUM */
   double num_float  = 1;       /* LISP OBJECT of type FLOAT  */
   BOOL   float_res = FALSE;    /* Ist das Ergebnis vom Typ FLOAT ? */
 
   for(i = 0; i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	num_fixnum *= GET_FIXNUM(STACK(base, i));
         break;
      case CL_FLOAT:
      	num_float *= GET_FLOAT(STACK(base, i));
      	float_res = TRUE;
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   if(float_res)
	{
      num_float *= num_fixnum;
      LOAD_FLOAT(make_float(STACK(base, 0), num_float), STACK(base, 0));
	}
   else
      LOAD_FIXNUM(num_fixnum, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * / number &rest more-numbers
 *----------------------------------------------------------------------------*/
void Fdiv(base, nargs)
CL_FORM *base;
int nargs;
{
   int i;
   long   num_fixnum = 1;       /* LISP OBJECT of type FIXNUM */
   long   div_fixnum;
   double num_float  = 1;       /* LISP OBJECT of type FLOAT  */
   double div_float;
   BOOL   float_res = FALSE;    /* Ist das Ergebnis eine FLOAT ? */
 
   if(nargs > 1)
	{
      switch(TYPE_OF(STACK(base, 0)))
      {
      case CL_FIXNUM:
      	num_fixnum = GET_FIXNUM(STACK(base, 0));
         break;
      case CL_FLOAT:
      	num_float = GET_FLOAT(STACK(base, 0));
      	float_res = TRUE;
         break;
      default: Lerror(STACK(base, 0), No_number);
      }
   }
   for(i =(nargs > 1 ? 1 : 0); i < nargs; i++)
	{
      switch(TYPE_OF(STACK(base, i)))
      {
      case CL_FIXNUM:
      	div_fixnum = GET_FIXNUM(STACK(base, i));
      	if(div_fixnum == 0)
      		goto div_by_zero;
      	if(! float_res)
         {
      		if(num_fixnum % div_fixnum == 0)
      			num_fixnum /= div_fixnum;
      		else
            {
      			num_float =(double)num_fixnum / div_fixnum;
      			float_res = TRUE;
            }
         }
      	else
         {
      		num_float /= div_fixnum;
         }
         break;
      case CL_FLOAT:
      	div_float = GET_FLOAT(STACK(base, i));
      	if(div_float == 0)
      		goto div_by_zero;
      	if(float_res)
      		num_float /= div_float;
      	else
         {
      		num_float = num_fixnum / div_float;
      		float_res = TRUE;
         }
         break;
      default: Lerror(STACK(base, i), No_number);
      }
   }
   if(float_res)
	{
      LOAD_FLOAT(make_float(STACK(base, 0), num_float), STACK(base, 0));
	}
   else
      LOAD_FIXNUM(num_fixnum, STACK(base, 0));
   return;

 div_by_zero:
	Labort("Division by zero");
}

/*------------------------------------------------------------------------------
 * 1+ number
 *----------------------------------------------------------------------------*/
void F1plus(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
      /* FIXNUM inkrementieren */
   	LOAD_FIXNUM(GET_FIXNUM(STACK(base, 0)) + 1, STACK(base,0)); 
      break;                    /* wird als Resultat zur"uckgeliefert */
   case CL_FLOAT:
      /* Eine neue Fliesskommazahl im Heap allozieren und 'number' + 1 dort
         abspeichern */
		LOAD_FLOAT(make_float(STACK(base, 0), GET_FLOAT(STACK(base, 0)) + 1),
		            STACK(base, 0));
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
}

/*------------------------------------------------------------------------------
 * 1- number
 *----------------------------------------------------------------------------*/
void F1minus(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
      /* FIXNUM dekrementieren */
   	LOAD_FIXNUM(GET_FIXNUM(STACK(base, 0)) - 1, STACK(base, 0)); 
      break;                    /* wird als Resultat zur"uckgeliefert */
   case CL_FLOAT:
      /* Eine neue Fliesskommazahl im Heap allozieren und 'number' - 1 dort
         abspeichern */
		LOAD_FLOAT(make_float(STACK(base, 0), GET_FLOAT(STACK(base, 0)) - 1),
		            STACK(base, 0));
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
}

/*------------------------------------------------------------------------------
 * Irrational and Transcendental Functions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Exponential and Logarithmic Functions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * expt base-number power-number
 *----------------------------------------------------------------------------*/
void expt_internal(base)
CL_FORM *base;
{
   double base_num, power;

   switch(TYPE_OF(STACK(base, 1)))
   {
   case CL_FIXNUM:
   {
   	long power_fixnum = GET_FIXNUM(STACK(base, 1));
      /*
        Falls 'power-number' = 0 vom Typ INTEGER,
        ist das Ergebnis 1 vom Typ von 'base-number'.
        */
      if(power_fixnum == 0L)
      {
         switch(TYPE_OF(STACK(base, 0)))
         {
         case CL_FIXNUM:
            LOAD_FIXNUM(1, STACK(base, 0));
            return;
         case CL_FLOAT:
            LOAD_FLOAT(make_float(STACK(base, 0), 1.0), STACK(base, 0));
            return;
         default: Lerror(STACK(base, 0), No_number);
         }
      }
      /*
        Sind 'base-number' und 'power-number' beide vom Typ RATIONAL
       (hier INTEGER), ist das Ergebnis exakt.
        */
      if(TYPE_OF(STACK(base, 0)) == CL_FIXNUM)
      {
      	long base_fixnum = GET_FIXNUM(STACK(base, 0));
      	long result      = 1;

      	while(power_fixnum-- > 0)
      		result *= base_fixnum;
      	LOAD_FIXNUM(result, STACK(base, 0));
      	return;
      }

      power = power_fixnum;
      break;
   }
   case CL_FLOAT:
      power = GET_FLOAT(STACK(base, 1));
      if(power == 0.0)
      {
         switch(TYPE_OF(STACK(base, 0)))
         {
         case CL_FIXNUM:
            if(GET_FIXNUM(STACK(base, 0)) == 0L)
               Lerror(STACK(base, 0), Illegal_base_num);
            LOAD_FIXNUM(1L, STACK(base, 0));
            return;
         case CL_FLOAT:
            if(GET_FLOAT(STACK(base, 0)) == 0.0)
               Lerror(STACK(base, 0), Illegal_base_num);
            LOAD_FLOAT(make_float(STACK(base, 0), 1.0), STACK(base, 0));
            return;
         default: Lerror(STACK(base, 0), No_number);
         }
      }
      break;
   default: Lerror(STACK(base, 1), No_number);
   }
   base_num = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), pow(base_num, power)),
              STACK(base, 0));
}
	
/*------------------------------------------------------------------------------
 * log number base
 *----------------------------------------------------------------------------*/
void log_internal(base)
CL_FORM *base;
{
   double number, base_num;

   number   = get_float(STACK(base, 0));
   base_num = get_float(STACK(base, 1));
   LOAD_FLOAT(make_float(STACK(base, 0), log(number) / log(base_num)),
               STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * sqrt number
 *----------------------------------------------------------------------------*/
void Fsqrt(base)
CL_FORM *base;
{     
   double number = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), sqrt(number)), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * Trigonometric and Related Functions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * sin radians
 *----------------------------------------------------------------------------*/
void Fsin(base)
CL_FORM *base;
{
   double radians = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), sin(radians)), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * cos radians
 *----------------------------------------------------------------------------*/
void Fcos(base)
CL_FORM *base;
{
   double radians = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), cos(radians)), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * tan radians
 *----------------------------------------------------------------------------*/
void Ftan(base)
CL_FORM *base;
{
   double radians = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), tan(radians)), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * asin radians
 *----------------------------------------------------------------------------*/
void Fasin(base)
CL_FORM *base;
{
   double radians = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), asin(radians)), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * acos radians
 *----------------------------------------------------------------------------*/
void Facos(base)
CL_FORM *base;
{
   double radians = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), acos(radians)), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * atan radians
 *----------------------------------------------------------------------------*/
void Fatan(base)
CL_FORM *base;
{
   double radians = get_float(STACK(base, 0));
   LOAD_FLOAT(make_float(STACK(base, 0), atan(radians)), STACK(base, 0));
}


/*------------------------------------------------------------------------------
 * Type Conversions and Component Extractions on Numbers
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * float number
 *----------------------------------------------------------------------------*/
void float_internal(base)
CL_FORM *base;
{
   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   {
   	double num_float = GET_FIXNUM(STACK(base, 0));
   	LOAD_FLOAT(make_float(STACK(base, 0), num_float), STACK(base, 0));
   	break;
   }
   case CL_FLOAT:
   	break;
   default: Lerror(STACK(base, 0), No_number);
   }
}

/*------------------------------------------------------------------------------
 * convert-to-int number divisor
 *----------------------------------------------------------------------------*/
void convert_to_int(base, ctype)
CL_FORM *base;
int ctype;
{
   double quotient, numerator, denominator;
   long   integer, fix_numerator, fix_denominator;
   BOOL   float_rest = FALSE;

   switch(TYPE_OF(STACK(base, 0)))
   {
   case CL_FIXNUM:
   	fix_numerator = GET_FIXNUM(STACK(base, 0));
      numerator = fix_numerator;
      break;
   case CL_FLOAT:
   	numerator  = GET_FLOAT(STACK(base, 0));
   	float_rest = TRUE;
      break;
   default: Lerror(STACK(base, 0), No_number);
   }
   switch(TYPE_OF(STACK(base, 1)))
   {
   case CL_FIXNUM:
   	fix_denominator = GET_FIXNUM(STACK(base, 1));
      denominator = fix_denominator;
      break;
   case CL_FLOAT:
   	denominator = GET_FLOAT(STACK(base, 1));
   	float_rest  = TRUE;
      break;
   default: Lerror(STACK(base, 1), No_number);
   }
   quotient = numerator / denominator;

   switch(ctype)
   {
	case 0:                      /* FLOOR */
		integer = floor(quotient);
		break;
	case 1:                      /* CEILING */
		integer = 1 + floor(quotient);
		break;
	case 2:                      /* TRUNCATE */
		integer = quotient;
		break;
	case 3:                      /* ROUND */
      integer = floor(quotient + 0.5);
      
      /* ROUND: x.5 zur geraden Zahl runden */
      if(integer == quotient + 0.5 && integer & 1)
         integer--;
      break;
	}

   LOAD_FIXNUM(integer, STACK(base, 0));
   if(float_rest)
	{
      LOAD_FLOAT(make_float(STACK(base, 1), numerator - integer * denominator),
                 STACK(base, 1));
	}
   else
	{
      LOAD_FIXNUM(fix_numerator - integer * fix_denominator, STACK(base, 1));
	}
   Fvalues(STACK(base, 0), 2);
}

/*------------------------------------------------------------------------------
 * floor-internal number divisor
 *----------------------------------------------------------------------------*/
void floor_internal(base)
CL_FORM *base;
{
   convert_to_int(STACK(base, 0), 0);
}

/*------------------------------------------------------------------------------
 * ceiling-internal number divisor
 *----------------------------------------------------------------------------*/
void ceiling_internal(base)
CL_FORM *base;
{
   convert_to_int(STACK(base, 0), 1);
}

/*------------------------------------------------------------------------------
 * truncate-internal number divisor
 *----------------------------------------------------------------------------*/
void truncate_internal(base)
CL_FORM *base;
{
   convert_to_int(STACK(base, 0), 2);
}

/*------------------------------------------------------------------------------
 * round-internal number divisor
 *----------------------------------------------------------------------------*/
void round_internal(base)
CL_FORM *base;
{
   convert_to_int(STACK(base, 0), 3);
}

/*------------------------------------------------------------------------------
 * integer-length integer
 *----------------------------------------------------------------------------*/
void Finteger_length(base)
CL_FORM *base;
{
   unsigned long i;
   int bits = 0;
   
   if(!CL_FIXNUMP(STACK(base, 0)))
      Lerror(STACK(base, 0), "~a is not a fixnum");

   i = (unsigned long)GET_FIXNUM(STACK(base, 0));
   if((long)i < 0)
      i = ~i;
   
   while(i)
   {
      bits++;
      i >>= 1;
   }
   LOAD_FIXNUM(bits, STACK(base, 0));
}
