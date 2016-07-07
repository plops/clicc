#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFremove_duplicates = {Fremove_duplicates, -2};

static void Z118_g1450(CL_FORM *base, int nargs);

void Fremove_duplicates(CL_FORM *base, int nargs)
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
	keysort(STACK(base, 1), nargs - 1, 6, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 7));
	if(NOT(supl_flags[3]))
	{
		LOAD_FIXNUM(0, STACK(base, 4));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_GLOBFUN(&CFidentity, STACK(base, 6));
	}
	COPY(STACK(base, 0), STACK(base, 7));
	Flength(STACK(base, 7));
	LOAD_NIL(STACK(base, 8));
	LOAD_NIL(STACK(base, 9));
	LOAD_NIL(STACK(base, 10));
	if(CL_TRUEP(STACK(base, 2)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 3))))
		{
			GEN_CLOSURE(array, STACK(base, 11), 4, Z118_g1450, -1);
			COPY(STACK(base, 3), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 11));
			COPY(STACK(base, 11), STACK(base, 2));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 2));
		}
	}
	COPY(STACK(base, 4), STACK(base, 11));
	COPY(STACK(base, 5), STACK(base, 12));
	COPY(STACK(base, 7), STACK(base, 13));
	check_seq_start_end(STACK(base, 11));
	COPY(STACK(base, 11), STACK(base, 5));
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 5), STACK(base, 11));
		F1minus(STACK(base, 11));
		M167_1:;
		COPY(STACK(base, 11), STACK(base, 12));
		COPY(STACK(base, 4), STACK(base, 13));
		Flt(STACK(base, 12), 2);
		if(CL_TRUEP(STACK(base, 12)))
		{
			goto RETURN181;
		}
		COPY(STACK(base, 11), STACK(base, 12));
		F1minus(STACK(base, 12));
		M168_1:;
		COPY(STACK(base, 12), STACK(base, 13));
		COPY(STACK(base, 4), STACK(base, 14));
		Flt(STACK(base, 13), 2);
		if(CL_TRUEP(STACK(base, 13)))
		{
			COPY(STACK(base, 0), STACK(base, 13));
			COPY(STACK(base, 11), STACK(base, 14));
			Felt(STACK(base, 13));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 14), 2);
				COPY(STACK(base, 13), CAR(lptr));
				COPY(STACK(base, 10), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 10));
			}
			goto RETURN182;
		}
		COPY(STACK(base, 2), STACK(base, 13));
		COPY(STACK(base, 6), STACK(base, 14));
		COPY(STACK(base, 0), STACK(base, 15));
		COPY(STACK(base, 11), STACK(base, 16));
		Felt(STACK(base, 15));
		Ffuncall(STACK(base, 14), 2);
		mv_count = 1;
		COPY(STACK(base, 6), STACK(base, 15));
		COPY(STACK(base, 0), STACK(base, 16));
		COPY(STACK(base, 12), STACK(base, 17));
		Felt(STACK(base, 16));
		Ffuncall(STACK(base, 15), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 13), 3);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 13)))
		{
			goto RETURN182;
		}
		F1minus(STACK(base, 12));
		goto M168_1;
		RETURN182:;
		F1minus(STACK(base, 11));
		goto M167_1;
		RETURN181:;
	}
	else
	{
		COPY(STACK(base, 4), STACK(base, 11));
		M169_1:;
		COPY(STACK(base, 11), STACK(base, 12));
		COPY(STACK(base, 5), STACK(base, 13));
		Fge(STACK(base, 12), 2);
		if(CL_TRUEP(STACK(base, 12)))
		{
			COPY(STACK(base, 10), STACK(base, 12));
			Freverse(STACK(base, 12));
			COPY(STACK(base, 12), STACK(base, 10));
			goto RETURN183;
		}
		COPY(STACK(base, 11), STACK(base, 12));
		F1plus(STACK(base, 12));
		M170_1:;
		COPY(STACK(base, 12), STACK(base, 13));
		COPY(STACK(base, 5), STACK(base, 14));
		Fge(STACK(base, 13), 2);
		if(CL_TRUEP(STACK(base, 13)))
		{
			COPY(STACK(base, 0), STACK(base, 13));
			COPY(STACK(base, 11), STACK(base, 14));
			Felt(STACK(base, 13));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 14), 2);
				COPY(STACK(base, 13), CAR(lptr));
				COPY(STACK(base, 10), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 10));
			}
			goto RETURN184;
		}
		COPY(STACK(base, 2), STACK(base, 13));
		COPY(STACK(base, 6), STACK(base, 14));
		COPY(STACK(base, 0), STACK(base, 15));
		COPY(STACK(base, 11), STACK(base, 16));
		Felt(STACK(base, 15));
		Ffuncall(STACK(base, 14), 2);
		mv_count = 1;
		COPY(STACK(base, 6), STACK(base, 15));
		COPY(STACK(base, 0), STACK(base, 16));
		COPY(STACK(base, 12), STACK(base, 17));
		Felt(STACK(base, 16));
		Ffuncall(STACK(base, 15), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 13), 3);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 13)))
		{
			goto RETURN184;
		}
		F1plus(STACK(base, 12));
		goto M170_1;
		RETURN184:;
		F1plus(STACK(base, 11));
		goto M169_1;
		RETURN183:;
	}
	COPY(STACK(base, 4), STACK(base, 11));
	COPY(STACK(base, 10), STACK(base, 12));
	Flength(STACK(base, 12));
	COPY(STACK(base, 7), STACK(base, 13));
	COPY(STACK(base, 5), STACK(base, 14));
	Fminus(STACK(base, 13), 2);
	Fplus(STACK(base, 11), 3);
	COPY(STACK(base, 11), STACK(base, 9));
	COPY(STACK(base, 0), STACK(base, 11));
	sequence_type(STACK(base, 11));
	COPY(STACK(base, 9), STACK(base, 12));
	Fmake_sequence(STACK(base, 11), 2);
	COPY(STACK(base, 11), STACK(base, 8));
	LOAD_FIXNUM(0, STACK(base, 11));
	COPY(STACK(base, 5), STACK(base, 12));
	M171_1:;
	COPY(STACK(base, 11), STACK(base, 13));
	COPY(STACK(base, 4), STACK(base, 14));
	Fge(STACK(base, 13), 2);
	if(CL_TRUEP(STACK(base, 13)))
	{
		goto RETURN185;
	}
	COPY(STACK(base, 0), STACK(base, 13));
	COPY(STACK(base, 11), STACK(base, 14));
	Felt(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 14));
	COPY(STACK(base, 8), STACK(base, 15));
	COPY(STACK(base, 11), STACK(base, 16));
	Fset_elt(STACK(base, 14));
	COPY(STACK(base, 11), STACK(base, 13));
	F1plus(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 11));
	goto M171_1;
	RETURN185:;
	M172_1:;
	if(CL_TRUEP(STACK(base, 10)))
	{
	}
	else
	{
		goto RETURN186;
	}
	COPY(STACK(base, 10), STACK(base, 13));
	Fcar(STACK(base, 13));
	COPY(STACK(base, 10), STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 15));
	Fcdr(STACK(base, 15));
	COPY(STACK(base, 15), STACK(base, 10));
	COPY(STACK(base, 13), STACK(base, 14));
	COPY(STACK(base, 8), STACK(base, 15));
	COPY(STACK(base, 11), STACK(base, 16));
	Fset_elt(STACK(base, 14));
	COPY(STACK(base, 11), STACK(base, 13));
	F1plus(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 11));
	goto M172_1;
	RETURN186:;
	M173_1:;
	COPY(STACK(base, 11), STACK(base, 13));
	COPY(STACK(base, 9), STACK(base, 14));
	Fge(STACK(base, 13), 2);
	if(CL_TRUEP(STACK(base, 13)))
	{
		goto RETURN187;
	}
	COPY(STACK(base, 0), STACK(base, 13));
	COPY(STACK(base, 12), STACK(base, 14));
	Felt(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 14));
	COPY(STACK(base, 8), STACK(base, 15));
	COPY(STACK(base, 11), STACK(base, 16));
	Fset_elt(STACK(base, 14));
	COPY(STACK(base, 11), STACK(base, 13));
	F1plus(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 11));
	F1plus(STACK(base, 12));
	goto M173_1;
	RETURN187:;
	COPY(STACK(base, 8), STACK(base, 0));
}

static void Z118_g1450(CL_FORM *base, int nargs)
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
