#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z10_g101(CL_FORM *base);
static void Z11_readc(CL_FORM *base);

void Fmake_concatenated_stream(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 1));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 91), STACK(base, 2));	/* CONCATENATED */
	LOAD_SYMBOL(SYMBOL(Slisp, 81), STACK(base, 3));	/* READC */
	{
		GEN_CLOSURE(array, STACK(base, 4), 4, Z11_readc, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 4));
	}
	COPY(STACK(base, 4), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 82), STACK(base, 5));	/* UNREADC */
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z10_g101, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 88), STACK(base, 7));	/* CLOSE */
	LOAD_GLOBFUN(&Cclose1, STACK(base, 8));
	make_stream(STACK(base, 1), 8);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z10_g101(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	Fcar(STACK(base, 2));
	stream_unreadc(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z11_readc(CL_FORM *base)
{
	if(CL_TRUEP(INDIRECT(GET_FORM(STACK(base, 0)) + 3)))
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
		Fcar(STACK(base, 1));
		stream_readc(STACK(base, 1));
		Ffuncall(STACK(base, 1), 1);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 1)))
		{
			COPY(STACK(base, 1), STACK(base, 0));
		}
		else
		{
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
			COPY(STACK(base, 2), STACK(base, 3));
			Fcdr(STACK(base, 3));
			COPY(STACK(base, 3), INDIRECT(GET_FORM(STACK(base, 0)) + 3));
			COPY(STACK(base, 0), STACK(base, 2));
			Z11_readc(STACK(base, 2));
			COPY(STACK(base, 2), STACK(base, 0));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
