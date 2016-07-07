#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fvector_push(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Ffill_pointer(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(0, STACK(base, 5));
	Farray_dimension(STACK(base, 4));
	Flt(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fset_aref(STACK(base, 3), 3);
		COPY(STACK(base, 2), STACK(base, 3));
		F1plus(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Fset_fill_pointer(STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
