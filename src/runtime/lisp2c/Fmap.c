#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z109_get_elem(CL_FORM *base);

void Fmap(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 3), nargs - 3);
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 2), CAR(lptr));
		COPY(STACK(base, 3), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 3));
	}
	LOAD_GLOBFUN(&CFmin, STACK(base, 4));
	LOAD_GLOBFUN(&CFlength, STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fmapcar(STACK(base, 5), 2);
	Fapply(STACK(base, 4), 2);
	mv_count = 1;
	LOAD_FIXNUM(0, STACK(base, 5));
	GEN_HEAPVAR(STACK(base, 5), STACK(base, 6));
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z109_get_elem, 1);
		COPY(STACK(base, 5), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 28))	/* LIST */
		{
			LOAD_NIL(STACK(base, 7));
			LOAD_NIL(STACK(base, 8));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 9), 2);
				COPY(STACK(base, 7), CAR(lptr));
				COPY(STACK(base, 8), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 7));
			}
			M151_1:;
			COPY(INDIRECT(STACK(base, 5)), STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 9));
			Fge(STACK(base, 8), 2);
			if(CL_TRUEP(STACK(base, 8)))
			{
				COPY(STACK(base, 7), STACK(base, 8));
				Fcar(STACK(base, 8));
				COPY(STACK(base, 8), STACK(base, 0));
				goto RETURN165;
			}
			COPY(STACK(base, 1), STACK(base, 8));
			COPY(STACK(base, 6), STACK(base, 9));
			COPY(STACK(base, 3), STACK(base, 10));
			Fmaplist(STACK(base, 9), 2);
			Fapply(STACK(base, 8), 2);
			mv_count = 1;
			COPY(STACK(base, 7), STACK(base, 9));
			add_q(STACK(base, 8));
			COPY(INDIRECT(STACK(base, 5)), STACK(base, 8));
			F1plus(STACK(base, 8));
			COPY(STACK(base, 8), INDIRECT(STACK(base, 5)));
			goto M151_1;
			RETURN165:;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 8));
			Fmake_sequence(STACK(base, 7), 2);
			M152_1:;
			COPY(INDIRECT(STACK(base, 5)), STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 9));
			Fge(STACK(base, 8), 2);
			if(CL_TRUEP(STACK(base, 8)))
			{
				COPY(STACK(base, 7), STACK(base, 0));
				goto RETURN166;
			}
			COPY(STACK(base, 7), STACK(base, 8));
			COPY(INDIRECT(STACK(base, 5)), STACK(base, 9));
			COPY(STACK(base, 1), STACK(base, 10));
			COPY(STACK(base, 6), STACK(base, 11));
			COPY(STACK(base, 3), STACK(base, 12));
			Fmaplist(STACK(base, 11), 2);
			Fapply(STACK(base, 10), 2);
			mv_count = 1;
			COPY(STACK(base, 10), STACK(base, 11));
			COPY(STACK(base, 8), STACK(base, 12));
			COPY(STACK(base, 9), STACK(base, 13));
			Fset_elt(STACK(base, 11));
			COPY(INDIRECT(STACK(base, 5)), STACK(base, 8));
			F1plus(STACK(base, 8));
			COPY(STACK(base, 8), INDIRECT(STACK(base, 5)));
			goto M152_1;
			RETURN166:;
		}
	}
	else
	{
		M153_1:;
		COPY(INDIRECT(STACK(base, 5)), STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 8));
		Fge(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN167;
		}
		COPY(STACK(base, 1), STACK(base, 7));
		COPY(STACK(base, 6), STACK(base, 8));
		COPY(STACK(base, 3), STACK(base, 9));
		Fmaplist(STACK(base, 8), 2);
		Fapply(STACK(base, 7), 2);
		mv_count = 1;
		COPY(INDIRECT(STACK(base, 5)), STACK(base, 7));
		F1plus(STACK(base, 7));
		COPY(STACK(base, 7), INDIRECT(STACK(base, 5)));
		goto M153_1;
		RETURN167:;
	}
}

static void Z109_get_elem(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Fcar(STACK(base, 2));
	if(CL_LISTP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 3));
		Fcar(STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 5));
		Fcdr(STACK(base, 5));
		COPY(STACK(base, 5), STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Fset_car(STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
		Felt(STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 0));
	}
}
