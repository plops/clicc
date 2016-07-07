#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Facons(CL_FORM *base)
{
	CL_FORM *lptr;
	lptr = form_alloc(STACK(base, 3), 2);
	COPY(STACK(base, 0), CAR(lptr));
	COPY(STACK(base, 1), CDR(lptr));
	LOAD_CONS(lptr, STACK(base, 3));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 3), CAR(lptr));
		COPY(STACK(base, 2), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 0));
	}
}
