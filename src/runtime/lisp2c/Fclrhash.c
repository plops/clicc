#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fclrhash(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_FIXNUM(5, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 3));	/* HASH-TABLE */
	struct_ref(STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_FIXNUM(1, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 5));	/* HASH-TABLE */
	LOAD_FIXNUM(0, STACK(base, 6));
	set_struct_ref(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 2));
	Flength(STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	M79_1:;
	COPY(STACK(base, 3), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fge(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		goto RETURN88;
	}
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fset_aref(STACK(base, 4), 3);
	F1plus(STACK(base, 3));
	goto M79_1;
	RETURN88:;
}
