#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z132_g1535(CL_FORM *base, int nargs);

void Fsubstitute(CL_FORM *base, int nargs)
{
	BOOL supl_flags[7];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 229),	/* FROM-END */
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
		SYMBOL(Slisp, 310),	/* COUNT */
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 3), nargs - 3, 7, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	GEN_HEAPVAR(STACK(base, 5), STACK(base, 10));
	if(NOT(supl_flags[3]))
	{
		LOAD_FIXNUM(0, STACK(base, 6));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 7));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 8));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_GLOBFUN(&CFidentity, STACK(base, 9));
	}
	COPY(STACK(base, 2), STACK(base, 10));
	Flength(STACK(base, 10));
	if(CL_TRUEP(STACK(base, 4)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 5))))
		{
			GEN_CLOSURE(array, STACK(base, 11), 4, Z132_g1535, -1);
			COPY(STACK(base, 5), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 11));
			COPY(STACK(base, 11), STACK(base, 4));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 4));
		}
	}
	COPY(STACK(base, 6), STACK(base, 11));
	COPY(STACK(base, 7), STACK(base, 12));
	COPY(STACK(base, 10), STACK(base, 13));
	check_seq_start_end(STACK(base, 11));
	COPY(STACK(base, 11), STACK(base, 7));
	COPY(STACK(base, 8), STACK(base, 11));
	COPY(STACK(base, 10), STACK(base, 12));
	check_seq_count(STACK(base, 11));
	COPY(STACK(base, 11), STACK(base, 8));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 2), STACK(base, 11));
		sequence_type(STACK(base, 11));
		COPY(STACK(base, 10), STACK(base, 12));
		Fmake_sequence(STACK(base, 11), 2);
		COPY(STACK(base, 10), STACK(base, 12));
		F1minus(STACK(base, 12));
		LOAD_FIXNUM(0, STACK(base, 13));
		M187_1:;
		COPY(STACK(base, 12), STACK(base, 14));
		Fminusp(STACK(base, 14));
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 11), STACK(base, 0));
			goto RETURN205;
		}
		COPY(STACK(base, 6), STACK(base, 14));
		COPY(STACK(base, 12), STACK(base, 15));
		Fle(STACK(base, 14), 2);
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 12), STACK(base, 14));
			COPY(STACK(base, 7), STACK(base, 15));
			Flt(STACK(base, 14), 2);
			if(CL_TRUEP(STACK(base, 14)))
			{
				COPY(STACK(base, 13), STACK(base, 14));
				COPY(STACK(base, 8), STACK(base, 15));
				Flt(STACK(base, 14), 2);
				if(CL_TRUEP(STACK(base, 14)))
				{
					COPY(STACK(base, 4), STACK(base, 14));
					COPY(STACK(base, 1), STACK(base, 15));
					COPY(STACK(base, 9), STACK(base, 16));
					COPY(STACK(base, 2), STACK(base, 17));
					COPY(STACK(base, 12), STACK(base, 18));
					Felt(STACK(base, 17));
					Ffuncall(STACK(base, 16), 2);
					mv_count = 1;
					Ffuncall(STACK(base, 14), 3);
					mv_count = 1;
					bool_result = CL_TRUEP(STACK(base, 14));
				}
				else
				{
					bool_result = FALSE;
				}
			}
			else
			{
				bool_result = FALSE;
			}
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 0), STACK(base, 14));
			COPY(STACK(base, 11), STACK(base, 15));
			COPY(STACK(base, 12), STACK(base, 16));
			Fset_elt(STACK(base, 14));
			COPY(STACK(base, 13), STACK(base, 14));
			F1plus(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 13));
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 14));
			COPY(STACK(base, 12), STACK(base, 15));
			Felt(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 15));
			COPY(STACK(base, 11), STACK(base, 16));
			COPY(STACK(base, 12), STACK(base, 17));
			Fset_elt(STACK(base, 15));
		}
		COPY(STACK(base, 12), STACK(base, 14));
		F1minus(STACK(base, 14));
		COPY(STACK(base, 14), STACK(base, 12));
		goto M187_1;
		RETURN205:;
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 11));
		sequence_type(STACK(base, 11));
		COPY(STACK(base, 10), STACK(base, 12));
		Fmake_sequence(STACK(base, 11), 2);
		LOAD_FIXNUM(0, STACK(base, 12));
		LOAD_FIXNUM(0, STACK(base, 13));
		M188_1:;
		COPY(STACK(base, 12), STACK(base, 14));
		COPY(STACK(base, 10), STACK(base, 15));
		Fge(STACK(base, 14), 2);
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 11), STACK(base, 0));
			goto RETURN206;
		}
		COPY(STACK(base, 6), STACK(base, 14));
		COPY(STACK(base, 12), STACK(base, 15));
		Fle(STACK(base, 14), 2);
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 12), STACK(base, 14));
			COPY(STACK(base, 7), STACK(base, 15));
			Flt(STACK(base, 14), 2);
			if(CL_TRUEP(STACK(base, 14)))
			{
				COPY(STACK(base, 13), STACK(base, 14));
				COPY(STACK(base, 8), STACK(base, 15));
				Flt(STACK(base, 14), 2);
				if(CL_TRUEP(STACK(base, 14)))
				{
					COPY(STACK(base, 4), STACK(base, 14));
					COPY(STACK(base, 1), STACK(base, 15));
					COPY(STACK(base, 9), STACK(base, 16));
					COPY(STACK(base, 2), STACK(base, 17));
					COPY(STACK(base, 12), STACK(base, 18));
					Felt(STACK(base, 17));
					Ffuncall(STACK(base, 16), 2);
					mv_count = 1;
					Ffuncall(STACK(base, 14), 3);
					mv_count = 1;
					bool_result = CL_TRUEP(STACK(base, 14));
				}
				else
				{
					bool_result = FALSE;
				}
			}
			else
			{
				bool_result = FALSE;
			}
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 0), STACK(base, 14));
			COPY(STACK(base, 11), STACK(base, 15));
			COPY(STACK(base, 12), STACK(base, 16));
			Fset_elt(STACK(base, 14));
			COPY(STACK(base, 13), STACK(base, 14));
			F1plus(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 13));
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 14));
			COPY(STACK(base, 12), STACK(base, 15));
			Felt(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 15));
			COPY(STACK(base, 11), STACK(base, 16));
			COPY(STACK(base, 12), STACK(base, 17));
			Fset_elt(STACK(base, 15));
		}
		COPY(STACK(base, 12), STACK(base, 14));
		F1plus(STACK(base, 14));
		COPY(STACK(base, 14), STACK(base, 12));
		goto M188_1;
		RETURN206:;
	}
}

static void Z132_g1535(CL_FORM *base, int nargs)
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
