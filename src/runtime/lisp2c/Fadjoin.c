#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z152_g1702(CL_FORM *base, int nargs);

void Fadjoin(CL_FORM *base, int nargs)
{
	BOOL supl_flags[3];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 2), nargs - 2, 3, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 5));
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(CL_TRUEP(STACK(base, 2)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 3))))
		{
			GEN_CLOSURE(array, STACK(base, 5), 4, Z152_g1702, -1);
			COPY(STACK(base, 3), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 2));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 2));
		}
	}
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 5));
		COPY(STACK(base, 0), STACK(base, 6));
		Ffuncall(STACK(base, 5), 2);
		mv_count = 1;
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 5));
	}
	COPY(STACK(base, 1), STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 7));	/* TEST */
	COPY(STACK(base, 2), STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 9));	/* KEY */
	COPY(STACK(base, 4), STACK(base, 10));
	Fmember(STACK(base, 5), 6);
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 5), 2);
		COPY(STACK(base, 0), CAR(lptr));
		COPY(STACK(base, 1), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 0));
	}
}

static void Z152_g1702(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 0);
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fapply(STACK(base, 2), 2);
	mv_count = 1;
	if(NOT(CL_TRUEP(STACK(base, 2))))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
