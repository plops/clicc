#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z16_g111(CL_FORM *base);
static void Z17_g110(CL_FORM *base);
static void Z18_g108(CL_FORM *base);
static void Z19_g106(CL_FORM *base);

void Fmake_echo_stream(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 2));
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 2));
	GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 3));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 93), STACK(base, 4));	/* ECHO */
	LOAD_SYMBOL(SYMBOL(Slisp, 81), STACK(base, 5));	/* READC */
	{
		GEN_CLOSURE(array, STACK(base, 6), 6, Z19_g106, 0);
		COPY(STACK(base, 1), &array[3]);
		COPY(STACK(base, 2), &array[4]);
		COPY(STACK(base, 0), &array[5]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 82), STACK(base, 7));	/* UNREADC */
	{
		GEN_CLOSURE(array, STACK(base, 8), 5, Z18_g108, 1);
		COPY(STACK(base, 2), &array[3]);
		COPY(STACK(base, 0), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 8));
	}
	COPY(STACK(base, 8), STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 83), STACK(base, 9));	/* WRITEC */
	{
		GEN_CLOSURE(array, STACK(base, 10), 4, Z17_g110, 1);
		COPY(STACK(base, 1), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 10));
	}
	COPY(STACK(base, 10), STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Slisp, 84), STACK(base, 11));	/* COLUMN */
	{
		GEN_CLOSURE(array, STACK(base, 12), 4, Z16_g111, 0);
		COPY(STACK(base, 1), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 12));
	}
	COPY(STACK(base, 12), STACK(base, 12));
	LOAD_SYMBOL(SYMBOL(Slisp, 88), STACK(base, 13));	/* CLOSE */
	LOAD_GLOBFUN(&Cclose1, STACK(base, 14));
	make_stream(STACK(base, 3), 12);
	COPY(STACK(base, 3), STACK(base, 0));
}

static void Z16_g111(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	stream_column(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z17_g110(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	stream_writec(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z18_g108(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
	stream_unreadc(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	mv_count = 1;
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	F1plus(STACK(base, 2));
	COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 3));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
}

static void Z19_g106(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 1));
	stream_readc(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	mv_count = 1;
	if(CL_TRUEP(STACK(base, 1)))
	{
		if(CL_FIXNUMP(INDIRECT(GET_FORM(STACK(base, 0)) + 4)) && GET_FIXNUM(INDIRECT(GET_FORM(STACK(base, 0)) + 4)) == 0)
		{
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
			stream_writec(STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 3));
			Ffuncall(STACK(base, 2), 2);
			mv_count = 1;
			COPY(STACK(base, 1), STACK(base, 0));
		}
		else
		{
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
			F1minus(STACK(base, 2));
			COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 4));
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 0));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
