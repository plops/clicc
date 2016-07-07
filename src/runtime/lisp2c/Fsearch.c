#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z131_g1523(CL_FORM *base, int nargs);

void Fsearch(CL_FORM *base, int nargs)
{
	BOOL supl_flags[8];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 229),	/* FROM-END */
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 139),	/* KEY */
		SYMBOL(Slisp, 237),	/* START1 */
		SYMBOL(Slisp, 211),	/* START2 */
		SYMBOL(Slisp, 238),	/* END1 */
		SYMBOL(Slisp, 212),	/* END2 */
	};
	keysort(STACK(base, 2), nargs - 2, 8, keylist, supl_flags, FALSE);
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
	GEN_HEAPVAR(STACK(base, 4), STACK(base, 10));
	if(NOT(supl_flags[3]))
	{
		LOAD_GLOBFUN(&CFidentity, STACK(base, 5));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_FIXNUM(0, STACK(base, 6));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_FIXNUM(0, STACK(base, 7));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_NIL(STACK(base, 8));
	}
	if(NOT(supl_flags[7]))
	{
		LOAD_NIL(STACK(base, 9));
	}
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 4))))
		{
			GEN_CLOSURE(array, STACK(base, 10), 4, Z131_g1523, -1);
			COPY(STACK(base, 4), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 10));
			COPY(STACK(base, 10), STACK(base, 3));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 3));
		}
	}
	COPY(STACK(base, 6), STACK(base, 10));
	COPY(STACK(base, 8), STACK(base, 11));
	COPY(STACK(base, 0), STACK(base, 12));
	Flength(STACK(base, 12));
	check_seq_start_end(STACK(base, 10));
	COPY(STACK(base, 10), STACK(base, 8));
	COPY(STACK(base, 7), STACK(base, 10));
	COPY(STACK(base, 9), STACK(base, 11));
	COPY(STACK(base, 1), STACK(base, 12));
	Flength(STACK(base, 12));
	check_seq_start_end(STACK(base, 10));
	COPY(STACK(base, 10), STACK(base, 9));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 5), STACK(base, 10));
		COPY(STACK(base, 0), STACK(base, 11));
		COPY(STACK(base, 8), STACK(base, 12));
		F1minus(STACK(base, 12));
		Felt(STACK(base, 11));
		Ffuncall(STACK(base, 10), 2);
		mv_count = 1;
	}
	else
	{
		COPY(STACK(base, 5), STACK(base, 10));
		COPY(STACK(base, 0), STACK(base, 11));
		COPY(STACK(base, 6), STACK(base, 12));
		Felt(STACK(base, 11));
		Ffuncall(STACK(base, 10), 2);
		mv_count = 1;
	}
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 9), STACK(base, 11));
		F1minus(STACK(base, 11));
		M183_1:;
		COPY(STACK(base, 11), STACK(base, 12));
		COPY(STACK(base, 7), STACK(base, 13));
		Fle(STACK(base, 12), 2);
		if(CL_TRUEP(STACK(base, 12)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN199;
		}
		COPY(STACK(base, 3), STACK(base, 12));
		COPY(STACK(base, 10), STACK(base, 13));
		COPY(STACK(base, 5), STACK(base, 14));
		COPY(STACK(base, 1), STACK(base, 15));
		COPY(STACK(base, 11), STACK(base, 16));
		Felt(STACK(base, 15));
		Ffuncall(STACK(base, 14), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 12), 3);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 12)))
		{
			LOAD_FIXNUM(2, STACK(base, 12));
			COPY(STACK(base, 8), STACK(base, 13));
			Fminus(STACK(base, 12), 2);
			COPY(STACK(base, 11), STACK(base, 13));
			F1minus(STACK(base, 13));
			M184_1:;
			COPY(STACK(base, 12), STACK(base, 14));
			COPY(STACK(base, 6), STACK(base, 15));
			Fle(STACK(base, 14), 2);
			if(CL_TRUEP(STACK(base, 14)))
			{
				bool_result = TRUE;
			}
			else
			{
				COPY(STACK(base, 13), STACK(base, 15));
				COPY(STACK(base, 7), STACK(base, 16));
				Fle(STACK(base, 15), 2);
				bool_result = CL_TRUEP(STACK(base, 15));
			}
			if(bool_result)
			{
				goto RETURN201;
			}
			COPY(STACK(base, 3), STACK(base, 14));
			COPY(STACK(base, 5), STACK(base, 15));
			COPY(STACK(base, 0), STACK(base, 16));
			COPY(STACK(base, 12), STACK(base, 17));
			Felt(STACK(base, 16));
			Ffuncall(STACK(base, 15), 2);
			mv_count = 1;
			COPY(STACK(base, 5), STACK(base, 16));
			COPY(STACK(base, 1), STACK(base, 17));
			COPY(STACK(base, 13), STACK(base, 18));
			Felt(STACK(base, 17));
			Ffuncall(STACK(base, 16), 2);
			mv_count = 1;
			Ffuncall(STACK(base, 14), 3);
			mv_count = 1;
			if(CL_TRUEP(STACK(base, 14)))
			{
			}
			else
			{
				COPY(STACK(base, 13), STACK(base, 14));
				F1plus(STACK(base, 14));
				COPY(STACK(base, 14), STACK(base, 11));
				goto RETURN200;
			}
			COPY(STACK(base, 12), STACK(base, 14));
			F1minus(STACK(base, 14));
			COPY(STACK(base, 13), STACK(base, 15));
			F1minus(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 13));
			COPY(STACK(base, 14), STACK(base, 12));
			goto M184_1;
			RETURN201:;
			COPY(STACK(base, 11), STACK(base, 12));
			F1plus(STACK(base, 12));
			COPY(STACK(base, 8), STACK(base, 13));
			COPY(STACK(base, 6), STACK(base, 14));
			Fminus(STACK(base, 13), 2);
			Fminus(STACK(base, 12), 2);
			COPY(STACK(base, 12), STACK(base, 0));
			goto RETURN198;
		}
		RETURN200:;
		COPY(STACK(base, 11), STACK(base, 12));
		F1minus(STACK(base, 12));
		COPY(STACK(base, 12), STACK(base, 11));
		goto M183_1;
		RETURN199:;
	}
	else
	{
		COPY(STACK(base, 7), STACK(base, 11));
		M185_1:;
		COPY(STACK(base, 11), STACK(base, 12));
		COPY(STACK(base, 9), STACK(base, 13));
		Fge(STACK(base, 12), 2);
		if(CL_TRUEP(STACK(base, 12)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN202;
		}
		COPY(STACK(base, 3), STACK(base, 12));
		COPY(STACK(base, 10), STACK(base, 13));
		COPY(STACK(base, 5), STACK(base, 14));
		COPY(STACK(base, 1), STACK(base, 15));
		COPY(STACK(base, 11), STACK(base, 16));
		Felt(STACK(base, 15));
		Ffuncall(STACK(base, 14), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 12), 3);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 12)))
		{
			COPY(STACK(base, 6), STACK(base, 12));
			F1plus(STACK(base, 12));
			COPY(STACK(base, 11), STACK(base, 13));
			F1plus(STACK(base, 13));
			M186_1:;
			COPY(STACK(base, 12), STACK(base, 14));
			COPY(STACK(base, 8), STACK(base, 15));
			Fge(STACK(base, 14), 2);
			if(CL_TRUEP(STACK(base, 14)))
			{
				bool_result = TRUE;
			}
			else
			{
				COPY(STACK(base, 13), STACK(base, 15));
				COPY(STACK(base, 9), STACK(base, 16));
				Fge(STACK(base, 15), 2);
				bool_result = CL_TRUEP(STACK(base, 15));
			}
			if(bool_result)
			{
				goto RETURN204;
			}
			COPY(STACK(base, 3), STACK(base, 14));
			COPY(STACK(base, 5), STACK(base, 15));
			COPY(STACK(base, 0), STACK(base, 16));
			COPY(STACK(base, 12), STACK(base, 17));
			Felt(STACK(base, 16));
			Ffuncall(STACK(base, 15), 2);
			mv_count = 1;
			COPY(STACK(base, 5), STACK(base, 16));
			COPY(STACK(base, 1), STACK(base, 17));
			COPY(STACK(base, 13), STACK(base, 18));
			Felt(STACK(base, 17));
			Ffuncall(STACK(base, 16), 2);
			mv_count = 1;
			Ffuncall(STACK(base, 14), 3);
			mv_count = 1;
			if(CL_TRUEP(STACK(base, 14)))
			{
			}
			else
			{
				COPY(STACK(base, 13), STACK(base, 14));
				F1minus(STACK(base, 14));
				COPY(STACK(base, 14), STACK(base, 11));
				goto RETURN203;
			}
			COPY(STACK(base, 12), STACK(base, 14));
			F1plus(STACK(base, 14));
			COPY(STACK(base, 13), STACK(base, 15));
			F1plus(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 13));
			COPY(STACK(base, 14), STACK(base, 12));
			goto M186_1;
			RETURN204:;
			COPY(STACK(base, 11), STACK(base, 0));
			goto RETURN198;
		}
		RETURN203:;
		COPY(STACK(base, 11), STACK(base, 12));
		F1plus(STACK(base, 12));
		COPY(STACK(base, 12), STACK(base, 11));
		goto M185_1;
		RETURN202:;
	}
	RETURN198:;
}

static void Z131_g1523(CL_FORM *base, int nargs)
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
