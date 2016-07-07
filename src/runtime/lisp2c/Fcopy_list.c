#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFcopy_list)
{
	MAKE_STRING(16, "~a is not a list"),	/* 0 */
};

static void Z137_f(CL_FORM *base);

void Fcopy_list(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_ATOMP(STACK(base, 0)))
		{
			LOAD_SMSTR((CL_FORM *)&KFcopy_list[0], STACK(base, 1));	/* ~a is not a list */
			COPY(STACK(base, 0), STACK(base, 2));
			Ferror(STACK(base, 1), 2);
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Z137_f(STACK(base, 1));
			COPY(STACK(base, 1), STACK(base, 0));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}

static void Z137_f(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		COPY(STACK(base, 0), STACK(base, 2));
		Fcdr(STACK(base, 2));
		Z137_f(STACK(base, 2));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 3), 2);
			COPY(STACK(base, 1), CAR(lptr));
			COPY(STACK(base, 2), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
