#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fvector_push_extend(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		COPY(STACK(base, 1), STACK(base, 2));
		Farray_total_size(STACK(base, 2));
		F1plus(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 1), STACK(base, 3));
	Ffill_pointer(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	Farray_total_size(STACK(base, 5));
	Fge(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Farray_total_size(STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Fplus(STACK(base, 5), 2);
		LOAD_SYMBOL(SYMBOL(Slisp, 97), STACK(base, 6));	/* FILL-POINTER */
		COPY(STACK(base, 3), STACK(base, 7));
		Fadjust_array(STACK(base, 4), 4);
	}
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fset_aref(STACK(base, 4), 3);
	COPY(STACK(base, 3), STACK(base, 4));
	F1plus(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	Fset_fill_pointer(STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 0));
}
