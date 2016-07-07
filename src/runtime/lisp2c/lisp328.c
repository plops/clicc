#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void vector_reverse(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Flength(STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 47), STACK(base, 2));	/* VECTOR */
	COPY(STACK(base, 0), STACK(base, 3));
	Farray_element_type(STACK(base, 3));
	Flist(STACK(base, 2), 2);
	COPY(STACK(base, 1), STACK(base, 3));
	Fmake_sequence(STACK(base, 2), 2);
	LOAD_FIXNUM(0, STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	F1minus(STACK(base, 4));
	M140_1:;
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	Fnumeql(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN154;
	}
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Felt(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	COPY(STACK(base, 4), STACK(base, 8));
	Fset_elt(STACK(base, 6));
	COPY(STACK(base, 3), STACK(base, 5));
	F1plus(STACK(base, 5));
	COPY(STACK(base, 4), STACK(base, 6));
	F1minus(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 4));
	COPY(STACK(base, 5), STACK(base, 3));
	goto M140_1;
	RETURN154:;
}
