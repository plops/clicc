#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void set_prop(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	get_prop(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 3), STACK(base, 4));
		Fcdr(STACK(base, 4));
		if(CL_CONSP(STACK(base, 4)))
		{
			COPY(STACK(base, 2), CAR(GET_FORM(STACK(base, 4))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[0], STACK(base, 5));	/* ~a is not a cons */
			COPY(STACK(base, 4), STACK(base, 6));
			Ferror(STACK(base, 5), 2);
		}
	}
	else
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 2), CAR(lptr));
		COPY(STACK(base, 0), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 4));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 5), 2);
			COPY(STACK(base, 1), CAR(lptr));
			COPY(STACK(base, 4), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
