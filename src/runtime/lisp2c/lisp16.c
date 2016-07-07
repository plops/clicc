#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z2_g96(CL_FORM *base);
static void Z3_g95(CL_FORM *base);
static void Z4_g94(CL_FORM *base);
static void Z5_g93(CL_FORM *base);
static void Z6_g92(CL_FORM *base);
static void Z7_g91(CL_FORM *base);
static void Z8_g90(CL_FORM *base);

void Fmake_synonym_stream(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 89), STACK(base, 1));	/* SYNONYM */
	COPY(INDIRECT(STACK(base, 0)), STACK(base, 2));
	{
		GEN_CLOSURE(array, STACK(base, 3), 4, Z8_g90, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 3));
	}
	COPY(STACK(base, 3), STACK(base, 3));
	{
		GEN_CLOSURE(array, STACK(base, 4), 4, Z7_g91, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 4));
	}
	COPY(STACK(base, 4), STACK(base, 4));
	{
		GEN_CLOSURE(array, STACK(base, 5), 4, Z6_g92, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 5));
	}
	COPY(STACK(base, 5), STACK(base, 5));
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z5_g93, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	{
		GEN_CLOSURE(array, STACK(base, 7), 4, Z4_g94, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 7));
	}
	COPY(STACK(base, 7), STACK(base, 7));
	{
		GEN_CLOSURE(array, STACK(base, 8), 4, Z3_g95, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 8));
	}
	COPY(STACK(base, 8), STACK(base, 8));
	{
		GEN_CLOSURE(array, STACK(base, 9), 4, Z2_g96, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 9));
	}
	COPY(STACK(base, 9), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 10));	/* STREAM */
	COPY(STACK(base, 1), STACK(base, 11));
	COPY(STACK(base, 2), STACK(base, 12));
	COPY(STACK(base, 3), STACK(base, 13));
	COPY(STACK(base, 4), STACK(base, 14));
	COPY(STACK(base, 5), STACK(base, 15));
	COPY(STACK(base, 6), STACK(base, 16));
	COPY(STACK(base, 7), STACK(base, 17));
	COPY(STACK(base, 8), STACK(base, 18));
	COPY(STACK(base, 9), STACK(base, 19));
	LOAD_GLOBFUN(&Cclose1, STACK(base, 20));
	make_struct(STACK(base, 10), 11);
	COPY(STACK(base, 10), STACK(base, 0));
}

static void Z2_g96(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Fsymbol_value(STACK(base, 1));
	stream_length(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z3_g95(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Fsymbol_value(STACK(base, 1));
	stream_seek(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z4_g94(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Fsymbol_value(STACK(base, 1));
	stream_tell(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z5_g93(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Fsymbol_value(STACK(base, 1));
	stream_column(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z6_g92(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	Fsymbol_value(STACK(base, 2));
	stream_writec(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z7_g91(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	Fsymbol_value(STACK(base, 2));
	stream_unreadc(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z8_g90(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	Fsymbol_value(STACK(base, 1));
	stream_readc(STACK(base, 1));
	Ffuncall(STACK(base, 1), 1);
	COPY(STACK(base, 1), STACK(base, 0));
}
