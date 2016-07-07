#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z12_g105(CL_FORM *base);
static void Z13_g104(CL_FORM *base);
static void Z14_g103(CL_FORM *base);
static void Z15_g102(CL_FORM *base);

void Fmake_two_way_stream(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 2));
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 2));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 92), STACK(base, 3));	/* TWO-WAY */
	LOAD_SYMBOL(SYMBOL(Slisp, 81), STACK(base, 4));	/* READC */
	{
		GEN_CLOSURE(array, STACK(base, 5), 4, Z15_g102, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 5));
	}
	COPY(STACK(base, 5), STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 82), STACK(base, 6));	/* UNREADC */
	{
		GEN_CLOSURE(array, STACK(base, 7), 4, Z14_g103, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 7));
	}
	COPY(STACK(base, 7), STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 83), STACK(base, 8));	/* WRITEC */
	{
		GEN_CLOSURE(array, STACK(base, 9), 4, Z13_g104, 0);
		COPY(STACK(base, 1), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 9));
	}
	COPY(STACK(base, 9), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 84), STACK(base, 10));	/* COLUMN */
	{
		GEN_CLOSURE(array, STACK(base, 11), 4, Z12_g105, 0);
		COPY(STACK(base, 1), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 11));
	}
	COPY(STACK(base, 11), STACK(base, 11));
	LOAD_SYMBOL(SYMBOL(Slisp, 88), STACK(base, 12));	/* CLOSE */
	LOAD_GLOBFUN(&Cclose1, STACK(base, 13));
	make_stream(STACK(base, 2), 12);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z12_g105(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	stream_column(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z13_g104(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	stream_writec(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z14_g103(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	stream_unreadc(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z15_g102(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	stream_readc(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}
