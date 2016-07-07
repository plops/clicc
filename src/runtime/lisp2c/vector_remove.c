#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cvector_remove = {vector_remove, -3};

static void Z113_g1428(CL_FORM *base, int nargs);

void vector_remove(CL_FORM *base, int nargs)
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
	keysort(STACK(base, 2), nargs - 2, 7, keylist, supl_flags, FALSE);
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
	GEN_HEAPVAR(STACK(base, 4), STACK(base, 9));
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
		LOAD_NIL(STACK(base, 7));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_GLOBFUN(&CFidentity, STACK(base, 8));
	}
	COPY(STACK(base, 1), STACK(base, 9));
	Flength(STACK(base, 9));
	LOAD_NIL(STACK(base, 10));
	LOAD_NIL(STACK(base, 11));
	LOAD_NIL(STACK(base, 12));
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 4))))
		{
			GEN_CLOSURE(array, STACK(base, 13), 4, Z113_g1428, -1);
			COPY(STACK(base, 4), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 13));
			COPY(STACK(base, 13), STACK(base, 3));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 3));
		}
	}
	COPY(STACK(base, 5), STACK(base, 13));
	COPY(STACK(base, 6), STACK(base, 14));
	COPY(STACK(base, 9), STACK(base, 15));
	check_seq_start_end(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 6));
	COPY(STACK(base, 7), STACK(base, 13));
	COPY(STACK(base, 9), STACK(base, 14));
	check_seq_count(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 7));
	COPY(STACK(base, 0), STACK(base, 13));
	COPY(STACK(base, 1), STACK(base, 14));
	COPY(STACK(base, 3), STACK(base, 15));
	COPY(STACK(base, 5), STACK(base, 16));
	COPY(STACK(base, 6), STACK(base, 17));
	COPY(STACK(base, 8), STACK(base, 18));
	LOAD_NIL(STACK(base, 19));
	COPY(STACK(base, 16), STACK(base, 20));
	M164_1:;
	COPY(STACK(base, 20), STACK(base, 21));
	COPY(STACK(base, 17), STACK(base, 22));
	Fge(STACK(base, 21), 2);
	if(CL_TRUEP(STACK(base, 21)))
	{
		COPY(STACK(base, 19), STACK(base, 21));
		Freverse(STACK(base, 21));
		COPY(STACK(base, 21), STACK(base, 10));
		goto RETURN178;
	}
	COPY(STACK(base, 15), STACK(base, 21));
	COPY(STACK(base, 13), STACK(base, 22));
	COPY(STACK(base, 18), STACK(base, 23));
	COPY(STACK(base, 14), STACK(base, 24));
	COPY(STACK(base, 20), STACK(base, 25));
	Felt(STACK(base, 24));
	Ffuncall(STACK(base, 23), 2);
	mv_count = 1;
	Ffuncall(STACK(base, 21), 3);
	mv_count = 1;
	if(CL_TRUEP(STACK(base, 21)))
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 21), 2);
		COPY(STACK(base, 20), CAR(lptr));
		COPY(STACK(base, 19), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 19));
	}
	COPY(STACK(base, 20), STACK(base, 21));
	F1plus(STACK(base, 21));
	COPY(STACK(base, 21), STACK(base, 20));
	goto M164_1;
	RETURN178:;
	COPY(STACK(base, 10), STACK(base, 13));
	Flength(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 11));
	COPY(STACK(base, 1), STACK(base, 13));
	sequence_type(STACK(base, 13));
	COPY(STACK(base, 9), STACK(base, 14));
	COPY(STACK(base, 11), STACK(base, 15));
	COPY(STACK(base, 7), STACK(base, 16));
	Fmin(STACK(base, 15), 2);
	Fminus(STACK(base, 14), 2);
	Fmake_sequence(STACK(base, 13), 2);
	COPY(STACK(base, 13), STACK(base, 12));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_FIXNUM(0, STACK(base, 13));
		LOAD_FIXNUM(0, STACK(base, 14));
		M165_1:;
		COPY(STACK(base, 13), STACK(base, 15));
		COPY(STACK(base, 9), STACK(base, 16));
		Fge(STACK(base, 15), 2);
		if(CL_TRUEP(STACK(base, 15)))
		{
			goto RETURN179;
		}
		COPY(STACK(base, 11), STACK(base, 15));
		COPY(STACK(base, 7), STACK(base, 16));
		Fle(STACK(base, 15), 2);
		if(CL_TRUEP(STACK(base, 15)))
		{
			COPY(STACK(base, 10), STACK(base, 15));
			Fcar(STACK(base, 15));
			bool_result = EQ(STACK(base, 13), STACK(base, 15)) || CL_FLOATP(STACK(base, 13)) && GET_FLOAT(STACK(base, 13)) == GET_FLOAT(STACK(base, 15));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 10), STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 16));
			Fcdr(STACK(base, 16));
			COPY(STACK(base, 16), STACK(base, 10));
			COPY(STACK(base, 11), STACK(base, 15));
			F1minus(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 11));
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 15));
			COPY(STACK(base, 13), STACK(base, 16));
			Felt(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 16));
			COPY(STACK(base, 12), STACK(base, 17));
			COPY(STACK(base, 14), STACK(base, 18));
			Fset_elt(STACK(base, 16));
			COPY(STACK(base, 10), STACK(base, 15));
			Fcar(STACK(base, 15));
			if(EQ(STACK(base, 13), STACK(base, 15)) || CL_FLOATP(STACK(base, 13)) && GET_FLOAT(STACK(base, 13)) == GET_FLOAT(STACK(base, 15)))
			{
				COPY(STACK(base, 10), STACK(base, 15));
				COPY(STACK(base, 15), STACK(base, 16));
				Fcdr(STACK(base, 16));
				COPY(STACK(base, 16), STACK(base, 10));
				COPY(STACK(base, 11), STACK(base, 15));
				F1minus(STACK(base, 15));
				COPY(STACK(base, 15), STACK(base, 11));
			}
			COPY(STACK(base, 14), STACK(base, 15));
			F1plus(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 14));
		}
		COPY(STACK(base, 13), STACK(base, 15));
		F1plus(STACK(base, 15));
		COPY(STACK(base, 15), STACK(base, 13));
		goto M165_1;
		RETURN179:;
	}
	else
	{
		LOAD_FIXNUM(0, STACK(base, 13));
		LOAD_FIXNUM(0, STACK(base, 14));
		M166_1:;
		COPY(STACK(base, 13), STACK(base, 15));
		COPY(STACK(base, 9), STACK(base, 16));
		Fge(STACK(base, 15), 2);
		if(CL_TRUEP(STACK(base, 15)))
		{
			goto RETURN180;
		}
		COPY(STACK(base, 7), STACK(base, 15));
		Fplusp(STACK(base, 15));
		if(CL_TRUEP(STACK(base, 15)))
		{
			COPY(STACK(base, 10), STACK(base, 15));
			Fcar(STACK(base, 15));
			bool_result = EQ(STACK(base, 13), STACK(base, 15)) || CL_FLOATP(STACK(base, 13)) && GET_FLOAT(STACK(base, 13)) == GET_FLOAT(STACK(base, 15));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 10), STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 16));
			Fcdr(STACK(base, 16));
			COPY(STACK(base, 16), STACK(base, 10));
			COPY(STACK(base, 7), STACK(base, 15));
			F1minus(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 7));
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 15));
			COPY(STACK(base, 13), STACK(base, 16));
			Felt(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 16));
			COPY(STACK(base, 12), STACK(base, 17));
			COPY(STACK(base, 14), STACK(base, 18));
			Fset_elt(STACK(base, 16));
			COPY(STACK(base, 14), STACK(base, 15));
			F1plus(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 14));
		}
		COPY(STACK(base, 13), STACK(base, 15));
		F1plus(STACK(base, 15));
		COPY(STACK(base, 15), STACK(base, 13));
		goto M166_1;
		RETURN180:;
	}
	COPY(STACK(base, 12), STACK(base, 0));
}

static void Z113_g1428(CL_FORM *base, int nargs)
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
