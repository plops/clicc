#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z25_g125(CL_FORM *base);
static void Z26_g124(CL_FORM *base);
static void Z27_g123(CL_FORM *base);
static void Z28_g122(CL_FORM *base);
static void Z29_g120(CL_FORM *base);

void Fmake_string_output_stream(CL_FORM *base, int nargs)
{
	if(nargs > 1)
	{
		goto ALL_OPT_SUPPLIED;
	}
	switch(nargs)
	{
		case 1:
		ALL_OPT_SUPPLIED:;
		GEN_HEAPVAR(STACK(base, 0), STACK(base, 0 + nargs));
	}
	switch(nargs)
	{
		case 0:
		LOAD_FIXNUM(10, STACK(base, 0));
		LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 1));	/* ELEMENT-TYPE */
		LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 2));	/* CHARACTER */
		LOAD_SYMBOL(SYMBOL(Slisp, 96), STACK(base, 3));	/* ADJUSTABLE */
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
		LOAD_SYMBOL(SYMBOL(Slisp, 97), STACK(base, 5));	/* FILL-POINTER */
		LOAD_FIXNUM(0, STACK(base, 6));
		Fmake_array(STACK(base, 0), 7);
		GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
		case 1:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	LOAD_FIXNUM(0, STACK(base, 1));
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 2));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 98), STACK(base, 3));	/* STRING-OUTPUT */
	LOAD_SYMBOL(SYMBOL(Slisp, 83), STACK(base, 4));	/* WRITEC */
	{
		GEN_CLOSURE(array, STACK(base, 5), 5, Z29_g120, 1);
		COPY(STACK(base, 0), &array[3]);
		COPY(STACK(base, 1), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 5));
	}
	COPY(STACK(base, 5), STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 84), STACK(base, 6));	/* COLUMN */
	{
		GEN_CLOSURE(array, STACK(base, 7), 4, Z28_g122, 0);
		COPY(STACK(base, 1), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 7));
	}
	COPY(STACK(base, 7), STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 85), STACK(base, 8));	/* TELL */
	{
		GEN_CLOSURE(array, STACK(base, 9), 4, Z27_g123, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 9));
	}
	COPY(STACK(base, 9), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 87), STACK(base, 10));	/* LENGTH */
	{
		GEN_CLOSURE(array, STACK(base, 11), 4, Z26_g124, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 11));
	}
	COPY(STACK(base, 11), STACK(base, 11));
	LOAD_SYMBOL(SYMBOL(Slisp, 80), STACK(base, 12));	/* EXTRA */
	{
		GEN_CLOSURE(array, STACK(base, 13), 4, Z25_g125, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 13));
	}
	COPY(STACK(base, 13), STACK(base, 13));
	LOAD_SYMBOL(SYMBOL(Slisp, 88), STACK(base, 14));	/* CLOSE */
	LOAD_GLOBFUN(&Cclose1, STACK(base, 15));
	make_stream(STACK(base, 2), 14);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z25_g125(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Flength(STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 2));	/* ELEMENT-TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 3));	/* CHARACTER */
	LOAD_SYMBOL(SYMBOL(Slisp, 99), STACK(base, 4));	/* INITIAL-CONTENTS */
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 5));
	Fmake_array(STACK(base, 1), 5);
	LOAD_FIXNUM(0, STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	Fset_fill_pointer(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z26_g124(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Flength(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z27_g123(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Flength(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z28_g122(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
}

static void Z29_g120(CL_FORM *base)
{
	if(CL_CHARP(STACK(base, 1)) && GET_CHAR(STACK(base, 1)) == '\n')
	{
		LOAD_FIXNUM(0, INDIRECT(GET_FORM(STACK(base, 0)) + 4));
	}
	else
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
		F1plus(STACK(base, 2));
		COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 4));
	}
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	Fvector_push_extend(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}
