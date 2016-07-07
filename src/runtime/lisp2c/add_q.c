#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void add_q(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 2));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 3), 2);
		COPY(STACK(base, 0), CAR(lptr));
		COPY(STACK(base, 2), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 2));
	}
	COPY(STACK(base, 1), STACK(base, 3));
	Fcdr(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 1), STACK(base, 3));
		Fcdr(STACK(base, 3));
		if(CL_CONSP(STACK(base, 3)))
		{
			COPY(STACK(base, 2), CDR(GET_FORM(STACK(base, 3))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[2], STACK(base, 4));	/* ~a is not a cons */
			COPY(STACK(base, 3), STACK(base, 5));
			Ferror(STACK(base, 4), 2);
		}
	}
	else
	{
		if(CL_CONSP(STACK(base, 1)))
		{
			COPY(STACK(base, 2), CAR(GET_FORM(STACK(base, 1))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[0], STACK(base, 3));	/* ~a is not a cons */
			COPY(STACK(base, 1), STACK(base, 4));
			Ferror(STACK(base, 3), 2);
		}
	}
	if(CL_CONSP(STACK(base, 1)))
	{
		COPY(STACK(base, 2), CDR(GET_FORM(STACK(base, 1))));
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[2], STACK(base, 0));	/* ~a is not a cons */
		Ferror(STACK(base, 0), 2);
	}
}
