#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kunix_namestring)
{
	MAKE_STRING(26, "~S is ambiguous:~{~%  ~A~}"),	/* 0 */
};

static void Z66_g624(CL_FORM *base);

void unix_namestring(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	LOAD_NIL(STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 2), CAR(lptr));
		COPY(STACK(base, 3), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 2));
	}
	GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
	{
		GEN_CLOSURE(array, STACK(base, 3), 4, Z66_g624, 1);
		COPY(STACK(base, 2), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 3));
	}
	COPY(STACK(base, 0), STACK(base, 4));
	Fpathname(STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	COPY(STACK(base, 3), STACK(base, 6));
	Penumerate_matches(STACK(base, 4));
	mv_count = 1;
	COPY(INDIRECT(STACK(base, 2)), STACK(base, 3));
	Fcar(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 3), STACK(base, 4));
		Fcdr(STACK(base, 4));
		if(CL_TRUEP(STACK(base, 4)))
		{
			LOAD_SMSTR((CL_FORM *)&Kunix_namestring[0], STACK(base, 4));	/* ~S is ambiguous:~{~%  ~A~} */
			COPY(STACK(base, 0), STACK(base, 5));
			COPY(STACK(base, 3), STACK(base, 6));
			Ferror(STACK(base, 4), 3);
		}
		COPY(STACK(base, 3), STACK(base, 4));
		Fcar(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}

static void Z66_g624(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	add_q(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}
