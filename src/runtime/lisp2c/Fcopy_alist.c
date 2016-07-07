#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFcopy_alist)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
};

static void Z138_f(CL_FORM *base);

void Fcopy_alist(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Z138_f(STACK(base, 1));
			COPY(STACK(base, 1), STACK(base, 0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFcopy_alist[0], STACK(base, 1));	/* etypecase: the value ~a is not a legal value */
			COPY(STACK(base, 0), STACK(base, 2));
			Ferror(STACK(base, 1), 2);
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}

static void Z138_f(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(CL_ATOMP(STACK(base, 1)))
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
			Fcar(STACK(base, 1));
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
			Fcdr(STACK(base, 2));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 3), 2);
				COPY(STACK(base, 1), CAR(lptr));
				COPY(STACK(base, 2), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 1));
			}
		}
		COPY(STACK(base, 0), STACK(base, 2));
		Fcdr(STACK(base, 2));
		Z138_f(STACK(base, 2));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 3), 2);
			COPY(STACK(base, 1), CAR(lptr));
			COPY(STACK(base, 2), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
