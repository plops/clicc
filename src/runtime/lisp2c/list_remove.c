#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Clist_remove = {list_remove, -3};

static void Z112_g1410(CL_FORM *base, int nargs);

void list_remove(CL_FORM *base, int nargs)
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
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 12), 2);
		COPY(STACK(base, 10), CAR(lptr));
		COPY(STACK(base, 11), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 10));
	}
	LOAD_FIXNUM(0, STACK(base, 11));
	LOAD_NIL(STACK(base, 12));
	LOAD_NIL(STACK(base, 13));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 14), 2);
		COPY(STACK(base, 12), CAR(lptr));
		COPY(STACK(base, 13), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 12));
	}
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 4))))
		{
			GEN_CLOSURE(array, STACK(base, 13), 4, Z112_g1410, -1);
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
	LOAD_FIXNUM(0, STACK(base, 13));
	LOAD_NIL(STACK(base, 14));
	COPY(STACK(base, 1), STACK(base, 15));
	M161_1:;
	if(CL_ATOMP(STACK(base, 15)))
	{
		LOAD_NIL(STACK(base, 14));
		goto RETURN175;
	}
	COPY(STACK(base, 15), STACK(base, 16));
	Fcar(STACK(base, 16));
	COPY(STACK(base, 16), STACK(base, 14));
	COPY(STACK(base, 13), STACK(base, 16));
	COPY(STACK(base, 6), STACK(base, 17));
	Fge(STACK(base, 16), 2);
	if(CL_TRUEP(STACK(base, 16)))
	{
		goto RETURN175;
	}
	COPY(STACK(base, 13), STACK(base, 16));
	COPY(STACK(base, 5), STACK(base, 17));
	Fge(STACK(base, 16), 2);
	if(CL_TRUEP(STACK(base, 16)))
	{
		COPY(STACK(base, 3), STACK(base, 16));
		COPY(STACK(base, 0), STACK(base, 17));
		COPY(STACK(base, 8), STACK(base, 18));
		COPY(STACK(base, 14), STACK(base, 19));
		Ffuncall(STACK(base, 18), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 16), 3);
		mv_count = 1;
		bool_result = CL_TRUEP(STACK(base, 16));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(STACK(base, 13), STACK(base, 16));
		COPY(STACK(base, 10), STACK(base, 17));
		add_q(STACK(base, 16));
		COPY(STACK(base, 11), STACK(base, 16));
		F1plus(STACK(base, 16));
		COPY(STACK(base, 16), STACK(base, 11));
	}
	COPY(STACK(base, 13), STACK(base, 16));
	F1plus(STACK(base, 16));
	COPY(STACK(base, 16), STACK(base, 13));
	COPY(STACK(base, 15), STACK(base, 16));
	Fcdr(STACK(base, 16));
	COPY(STACK(base, 16), STACK(base, 15));
	goto M161_1;
	RETURN175:;
	COPY(STACK(base, 10), STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 14));
	Fcar(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 10));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_FIXNUM(0, STACK(base, 13));
		M162_1:;
		COPY(STACK(base, 13), STACK(base, 14));
		COPY(STACK(base, 9), STACK(base, 15));
		Fge(STACK(base, 14), 2);
		if(CL_TRUEP(STACK(base, 14)))
		{
			goto RETURN176;
		}
		COPY(STACK(base, 11), STACK(base, 14));
		COPY(STACK(base, 7), STACK(base, 15));
		Fle(STACK(base, 14), 2);
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 10), STACK(base, 14));
			Fcar(STACK(base, 14));
			bool_result = EQ(STACK(base, 13), STACK(base, 14)) || CL_FLOATP(STACK(base, 13)) && GET_FLOAT(STACK(base, 13)) == GET_FLOAT(STACK(base, 14));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 10), STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 15));
			Fcdr(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 10));
			COPY(STACK(base, 11), STACK(base, 14));
			F1minus(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 11));
			COPY(STACK(base, 1), STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 15));
			Fcdr(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 1));
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 14));
			Fcar(STACK(base, 14));
			COPY(STACK(base, 1), STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 16));
			Fcdr(STACK(base, 16));
			COPY(STACK(base, 16), STACK(base, 1));
			COPY(STACK(base, 12), STACK(base, 15));
			add_q(STACK(base, 14));
			COPY(STACK(base, 10), STACK(base, 14));
			Fcar(STACK(base, 14));
			if(EQ(STACK(base, 13), STACK(base, 14)) || CL_FLOATP(STACK(base, 13)) && GET_FLOAT(STACK(base, 13)) == GET_FLOAT(STACK(base, 14)))
			{
				COPY(STACK(base, 10), STACK(base, 14));
				COPY(STACK(base, 14), STACK(base, 15));
				Fcdr(STACK(base, 15));
				COPY(STACK(base, 15), STACK(base, 10));
				COPY(STACK(base, 11), STACK(base, 14));
				F1minus(STACK(base, 14));
				COPY(STACK(base, 14), STACK(base, 11));
			}
		}
		COPY(STACK(base, 13), STACK(base, 14));
		F1plus(STACK(base, 14));
		COPY(STACK(base, 14), STACK(base, 13));
		goto M162_1;
		RETURN176:;
	}
	else
	{
		LOAD_FIXNUM(0, STACK(base, 13));
		M163_1:;
		COPY(STACK(base, 13), STACK(base, 14));
		COPY(STACK(base, 9), STACK(base, 15));
		Fge(STACK(base, 14), 2);
		if(CL_TRUEP(STACK(base, 14)))
		{
			goto RETURN177;
		}
		COPY(STACK(base, 7), STACK(base, 14));
		Fplusp(STACK(base, 14));
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 10), STACK(base, 14));
			Fcar(STACK(base, 14));
			bool_result = EQ(STACK(base, 13), STACK(base, 14)) || CL_FLOATP(STACK(base, 13)) && GET_FLOAT(STACK(base, 13)) == GET_FLOAT(STACK(base, 14));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 10), STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 15));
			Fcdr(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 10));
			COPY(STACK(base, 7), STACK(base, 14));
			F1minus(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 15));
			Fcdr(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 1));
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 14));
			Fcar(STACK(base, 14));
			COPY(STACK(base, 1), STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 16));
			Fcdr(STACK(base, 16));
			COPY(STACK(base, 16), STACK(base, 1));
			COPY(STACK(base, 12), STACK(base, 15));
			add_q(STACK(base, 14));
		}
		COPY(STACK(base, 13), STACK(base, 14));
		F1plus(STACK(base, 14));
		COPY(STACK(base, 14), STACK(base, 13));
		goto M163_1;
		RETURN177:;
	}
	COPY(STACK(base, 12), STACK(base, 13));
	Fcar(STACK(base, 13));
	COPY(STACK(base, 13), STACK(base, 0));
}

static void Z112_g1410(CL_FORM *base, int nargs)
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
