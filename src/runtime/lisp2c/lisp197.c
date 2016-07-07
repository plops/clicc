#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFfind_all_symbols)
{
	MAKE_SYMREF(SYMBOL(Slisp, 305)),	/* 0 CONS(0) INTERNAL */
	MAKE_LIST(&KFfind_all_symbols[2]),
	MAKE_SYMREF(SYMBOL(Slisp, 306)),	/* CONS(2) EXTERNAL */
	MAKE_NIL,
};

static void Z81_g848(CL_FORM *base);

void Ffind_all_symbols(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	LOAD_NIL(STACK(base, 1));
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
	if(CL_SYMBOLP(INDIRECT(STACK(base, 0))) || CL_NILP(INDIRECT(STACK(base, 0))))
	{
		COPY(INDIRECT(STACK(base, 0)), STACK(base, 2));
		Fsymbol_name(STACK(base, 2));
		COPY(STACK(base, 2), INDIRECT(STACK(base, 0)));
	}
	LOAD_NIL(STACK(base, 2));
	{
		GEN_CLOSURE(array, STACK(base, 3), 5, Z81_g848, 1);
		COPY(STACK(base, 1), &array[3]);
		COPY(STACK(base, 0), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 3));
	}
	COPY(STACK(base, 3), STACK(base, 3));
	COPY(SYMVAL(Slisp, 302), STACK(base, 4));	/* *PACKAGE-ARRAY* */
	Fmap(STACK(base, 2), 3);
	LOAD_NIL(STACK(base, 0));
}

static void Z81_g848(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffind_symbol(STACK(base, 2), 2);
	COPY(&mv_buf[0], STACK(base, 3));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 2));
			case 1:
			LOAD_NIL(STACK(base, 3));
			nargs = 2;
		}
		Flist(STACK(base, 4), nargs - 2);
		COPY(STACK(base, 3), STACK(base, 5));
		LOAD_CONS((CL_FORM *)&KFfind_all_symbols[0], STACK(base, 6));
		Fmember(STACK(base, 5), 2);
		if(CL_TRUEP(STACK(base, 5)))
		{
			COPY(STACK(base, 2), STACK(base, 5));
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 6));
			Fadjoin(STACK(base, 5), 2);
			COPY(STACK(base, 5), INDIRECT(GET_FORM(STACK(base, 0)) + 3));
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
}
