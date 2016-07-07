#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Carray_reader = {array_reader, 3};

CL_INIT2(Karray_reader)
{
	MAKE_STRING(37, "Rank argument must be supplied to #~a"),	/* 0 */
};

static void Z90_calc_dim(CL_FORM *base);

void array_reader(CL_FORM *base)
{
	if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
	{
		COPY(STACK(base, 0), STACK(base, 3));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
		LOAD_NIL(STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		Fread(STACK(base, 3), 4);
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		if(CL_TRUEP(STACK(base, 2)))
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
			LOAD_NIL(STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
			Fread(STACK(base, 3), 4);
			COPY(STACK(base, 2), STACK(base, 4));
			COPY(STACK(base, 3), STACK(base, 5));
			Z90_calc_dim(STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 99), STACK(base, 5));	/* INITIAL-CONTENTS */
			COPY(STACK(base, 3), STACK(base, 6));
			Fmake_array(STACK(base, 4), 3);
			COPY(STACK(base, 4), STACK(base, 0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Karray_reader[0], STACK(base, 3));	/* Rank argument must be supplied to #~a */
			COPY(STACK(base, 1), STACK(base, 4));
			Ferror(STACK(base, 3), 2);
		}
	}
}

static void Z90_calc_dim(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 0)
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 2));
		Flength(STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		F1minus(STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Fcar(STACK(base, 4));
		Z90_calc_dim(STACK(base, 3));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 4), 2);
			COPY(STACK(base, 2), CAR(lptr));
			COPY(STACK(base, 3), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
