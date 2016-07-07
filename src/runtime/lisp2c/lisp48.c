#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Farray_dimensions(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Farray_rank(STACK(base, 1));
	LOAD_NIL(STACK(base, 2));
	M7_1:;
	COPY(STACK(base, 1), STACK(base, 3));
	Fzerop(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN7;
	}
	COPY(STACK(base, 1), STACK(base, 3));
	F1minus(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Farray_dimension(STACK(base, 3));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 3), CAR(lptr));
		COPY(STACK(base, 2), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 2));
	}
	goto M7_1;
	RETURN7:;
}
