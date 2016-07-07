#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z128_g1512(CL_FORM *base);

void Fcount_if(CL_FORM *base, int nargs)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 0 + nargs));
	{
		BOOL supl_flags[4];
		static CL_FORM * keylist[] =
		{
			SYMBOL(Slisp, 229),	/* FROM-END */
			SYMBOL(Slisp, 161),	/* START */
			SYMBOL(Slisp, 162),	/* END */
			SYMBOL(Slisp, 139),	/* KEY */
		};
		keysort(STACK(base, 2), nargs - 2, 4, keylist, supl_flags, FALSE);
		Flist(STACK(base, 6), nargs - 2);
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
	}
	LOAD_GLOBFUN(&CFcount, STACK(base, 7));
	LOAD_NIL(STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 10));	/* TEST */
	{
		GEN_CLOSURE(array, STACK(base, 11), 4, Z128_g1512, 2);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 11));
	}
	COPY(STACK(base, 11), STACK(base, 11));
	COPY(STACK(base, 6), STACK(base, 12));
	Fapply(STACK(base, 7), 6);
	COPY(STACK(base, 7), STACK(base, 0));
}

static void Z128_g1512(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	Ffuncall(STACK(base, 3), 2);
	COPY(STACK(base, 3), STACK(base, 0));
}
