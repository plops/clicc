#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z117_g1448(CL_FORM *base);

void Fdelete_if_not(CL_FORM *base, int nargs)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 0 + nargs));
	{
		BOOL supl_flags[5];
		static CL_FORM * keylist[] =
		{
			SYMBOL(Slisp, 229),	/* FROM-END */
			SYMBOL(Slisp, 161),	/* START */
			SYMBOL(Slisp, 162),	/* END */
			SYMBOL(Slisp, 310),	/* COUNT */
			SYMBOL(Slisp, 139),	/* KEY */
		};
		keysort(STACK(base, 2), nargs - 2, 5, keylist, supl_flags, FALSE);
		Flist(STACK(base, 7), nargs - 2);
		if(NOT(supl_flags[0]))
		{
			LOAD_NIL(STACK(base, 2));
		}
		if(NOT(supl_flags[1]))
		{
			LOAD_NIL(STACK(base, 3));
		}
		if(NOT(supl_flags[2]))
		{
			LOAD_NIL(STACK(base, 4));
		}
		if(NOT(supl_flags[3]))
		{
			LOAD_NIL(STACK(base, 5));
		}
		if(NOT(supl_flags[4]))
		{
			LOAD_NIL(STACK(base, 6));
		}
	}
	LOAD_GLOBFUN(&CFremove, STACK(base, 8));
	LOAD_NIL(STACK(base, 9));
	COPY(STACK(base, 1), STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 11));	/* TEST */
	{
		GEN_CLOSURE(array, STACK(base, 12), 4, Z117_g1448, 2);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 12));
	}
	COPY(STACK(base, 12), STACK(base, 12));
	COPY(STACK(base, 7), STACK(base, 13));
	Fapply(STACK(base, 8), 6);
	COPY(STACK(base, 8), STACK(base, 0));
}

static void Z117_g1448(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	Ffuncall(STACK(base, 3), 2);
	mv_count = 1;
	if(NOT(CL_TRUEP(STACK(base, 3))))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
