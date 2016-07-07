#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFevery = {Fevery, -3};

static void Z111_get_elem(CL_FORM *base);

void Fevery(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 2), nargs - 2);
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 3), 2);
		COPY(STACK(base, 1), CAR(lptr));
		COPY(STACK(base, 2), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 2));
	}
	LOAD_GLOBFUN(&CFmin, STACK(base, 3));
	LOAD_GLOBFUN(&CFlength, STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fmapcar(STACK(base, 4), 2);
	Fapply(STACK(base, 3), 2);
	mv_count = 1;
	LOAD_FIXNUM(0, STACK(base, 4));
	GEN_HEAPVAR(STACK(base, 4), STACK(base, 5));
	{
		GEN_CLOSURE(array, STACK(base, 5), 4, Z111_get_elem, 1);
		COPY(STACK(base, 4), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 5));
	}
	M155_1:;
	COPY(INDIRECT(STACK(base, 4)), STACK(base, 6));
	COPY(STACK(base, 3), STACK(base, 7));
	Fge(STACK(base, 6), 2);
	if(CL_TRUEP(STACK(base, 6)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
		goto RETURN169;
	}
	COPY(STACK(base, 0), STACK(base, 6));
	COPY(STACK(base, 5), STACK(base, 7));
	COPY(STACK(base, 2), STACK(base, 8));
	Fmaplist(STACK(base, 7), 2);
	Fapply(STACK(base, 6), 2);
	mv_count = 1;
	if(CL_TRUEP(STACK(base, 6)))
	{
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN169;
	}
	COPY(INDIRECT(STACK(base, 4)), STACK(base, 6));
	F1plus(STACK(base, 6));
	COPY(STACK(base, 6), INDIRECT(STACK(base, 4)));
	goto M155_1;
	RETURN169:;
}

static void Z111_get_elem(CL_FORM *base)
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
