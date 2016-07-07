#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void array_rmi_internal(CL_FORM *base)
{
	if(CL_ARRAY_P(STACK(base, 0)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 121), STACK(base, 2));	/* NO_ARRAY */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	LOAD_FIXNUM(0, STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	M9_1:;
	if(CL_ATOMP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 4));
		goto RETURN9;
	}
	COPY(STACK(base, 5), STACK(base, 6));
	Fcar(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	Farray_dimension(STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	F1plus(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 2));
	COPY(STACK(base, 3), STACK(base, 7));
	COPY(STACK(base, 6), STACK(base, 8));
	Fmult(STACK(base, 7), 2);
	COPY(STACK(base, 4), STACK(base, 8));
	Fplus(STACK(base, 7), 2);
	COPY(STACK(base, 7), STACK(base, 3));
	Fcdr(STACK(base, 5));
	goto M9_1;
	RETURN9:;
	COPY(STACK(base, 3), STACK(base, 0));
}
