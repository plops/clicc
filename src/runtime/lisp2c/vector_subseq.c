#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void vector_subseq(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	Flength(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	check_seq_start_end(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 47), STACK(base, 4));	/* VECTOR */
	COPY(STACK(base, 0), STACK(base, 5));
	Farray_element_type(STACK(base, 5));
	Flist(STACK(base, 4), 2);
	COPY(STACK(base, 2), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	Fminus(STACK(base, 5), 2);
	Fmake_sequence(STACK(base, 4), 2);
	COPY(STACK(base, 1), STACK(base, 5));
	LOAD_FIXNUM(0, STACK(base, 6));
	M137_1:;
	COPY(STACK(base, 5), STACK(base, 7));
	COPY(STACK(base, 2), STACK(base, 8));
	Fnumeql(STACK(base, 7), 2);
	if(CL_TRUEP(STACK(base, 7)))
	{
		COPY(STACK(base, 4), STACK(base, 0));
		goto RETURN151;
	}
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Felt(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 8));
	COPY(STACK(base, 4), STACK(base, 9));
	COPY(STACK(base, 6), STACK(base, 10));
	Fset_elt(STACK(base, 8));
	COPY(STACK(base, 5), STACK(base, 7));
	F1plus(STACK(base, 7));
	COPY(STACK(base, 6), STACK(base, 8));
	F1plus(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 6));
	COPY(STACK(base, 7), STACK(base, 5));
	goto M137_1;
	RETURN151:;
}
