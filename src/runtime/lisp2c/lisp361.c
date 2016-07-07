#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cvector_position = {vector_position, -3};

static void Z123_g1496(CL_FORM *base, int nargs);

void vector_position(CL_FORM *base, int nargs)
{
	BOOL supl_flags[6];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 229),	/* FROM-END */
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 2), nargs - 2, 6, keylist, supl_flags, FALSE);
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
	GEN_HEAPVAR(STACK(base, 4), STACK(base, 8));
	if(NOT(supl_flags[3]))
	{
		LOAD_FIXNUM(0, STACK(base, 5));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 6));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_GLOBFUN(&CFidentity, STACK(base, 7));
	}
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 4))))
		{
			GEN_CLOSURE(array, STACK(base, 8), 4, Z123_g1496, -1);
			COPY(STACK(base, 4), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 8));
			COPY(STACK(base, 8), STACK(base, 3));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 3));
		}
	}
	COPY(STACK(base, 5), STACK(base, 8));
	COPY(STACK(base, 6), STACK(base, 9));
	COPY(STACK(base, 1), STACK(base, 10));
	Flength(STACK(base, 10));
	check_seq_start_end(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 6));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 6), STACK(base, 8));
		F1minus(STACK(base, 8));
		M177_1:;
		COPY(STACK(base, 8), STACK(base, 9));
		COPY(STACK(base, 5), STACK(base, 10));
		Flt(STACK(base, 9), 2);
		if(CL_TRUEP(STACK(base, 9)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN191;
		}
		COPY(STACK(base, 3), STACK(base, 9));
		COPY(STACK(base, 0), STACK(base, 10));
		COPY(STACK(base, 7), STACK(base, 11));
		COPY(STACK(base, 1), STACK(base, 12));
		COPY(STACK(base, 8), STACK(base, 13));
		Felt(STACK(base, 12));
		Ffuncall(STACK(base, 11), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 9), 3);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 9)))
		{
			COPY(STACK(base, 8), STACK(base, 0));
			goto RETURN191;
		}
		F1minus(STACK(base, 8));
		goto M177_1;
		RETURN191:;
	}
	else
	{
		COPY(STACK(base, 5), STACK(base, 8));
		M178_1:;
		COPY(STACK(base, 8), STACK(base, 9));
		COPY(STACK(base, 6), STACK(base, 10));
		Fge(STACK(base, 9), 2);
		if(CL_TRUEP(STACK(base, 9)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN192;
		}
		COPY(STACK(base, 3), STACK(base, 9));
		COPY(STACK(base, 0), STACK(base, 10));
		COPY(STACK(base, 7), STACK(base, 11));
		COPY(STACK(base, 1), STACK(base, 12));
		COPY(STACK(base, 8), STACK(base, 13));
		Felt(STACK(base, 12));
		Ffuncall(STACK(base, 11), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 9), 3);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 9)))
		{
			COPY(STACK(base, 8), STACK(base, 0));
			goto RETURN192;
		}
		F1plus(STACK(base, 8));
		goto M178_1;
		RETURN192:;
	}
}

static void Z123_g1496(CL_FORM *base, int nargs)
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
