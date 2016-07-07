#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z9_g97(CL_FORM *base);

void Fmake_broadcast_stream(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 1));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 90), STACK(base, 2));	/* BROADCAST */
	LOAD_SYMBOL(SYMBOL(Slisp, 83), STACK(base, 3));	/* WRITEC */
	{
		GEN_CLOSURE(array, STACK(base, 4), 4, Z9_g97, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 4));
	}
	COPY(STACK(base, 4), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 88), STACK(base, 5));	/* CLOSE */
	LOAD_GLOBFUN(&Cclose1, STACK(base, 6));
	make_stream(STACK(base, 1), 6);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z9_g97(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	M1_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN1;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	stream_writec(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	Ffuncall(STACK(base, 4), 2);
	mv_count = 1;
	Fcdr(STACK(base, 3));
	goto M1_1;
	RETURN1:;
}
