#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z84_g930(CL_FORM *base);

void Fmaphash(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 2));
	LOAD_NIL(STACK(base, 2));
	{
		GEN_CLOSURE(array, STACK(base, 3), 4, Z84_g930, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 3));
	}
	COPY(STACK(base, 3), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(5, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 6));	/* HASH-TABLE */
	struct_ref(STACK(base, 4));
	Fmap(STACK(base, 2), 3);
	LOAD_NIL(STACK(base, 0));
}

static void Z85_g931(CL_FORM *base);

static void Z84_g930(CL_FORM *base)
{
	GEN_CLOSURE(array, STACK(base, 2), 4, Z85_g931, 1);
	COPY(GET_FORM(STACK(base, 0)) + 3, &array[3]);
	LOAD_CLOSURE(array, STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fmapc(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z85_g931(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fcar(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Fcdr(STACK(base, 4));
	Ffuncall(STACK(base, 2), 3);
	COPY(STACK(base, 2), STACK(base, 0));
}
