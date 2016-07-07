#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Frevappend(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	M196_1:;
	COPY(STACK(base, 2), STACK(base, 4));
	Fendp(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 3), STACK(base, 0));
		goto RETURN215;
	}
	COPY(STACK(base, 2), STACK(base, 4));
	Fcdr(STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fcar(STACK(base, 5));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 6), 2);
		COPY(STACK(base, 5), CAR(lptr));
		COPY(STACK(base, 3), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 3));
	}
	COPY(STACK(base, 4), STACK(base, 2));
	goto M196_1;
	RETURN215:;
}
