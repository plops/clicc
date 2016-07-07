#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fgethash(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	internal_sxhash(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(0, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 6));	/* HASH-TABLE */
	struct_ref(STACK(base, 4));
	Fmod(STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	LOAD_FIXNUM(5, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 7));	/* HASH-TABLE */
	struct_ref(STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	vref(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 6));	/* TEST */
	COPY(STACK(base, 1), STACK(base, 7));
	LOAD_FIXNUM(4, STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 9));	/* HASH-TABLE */
	struct_ref(STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 8));	/* KEY */
	LOAD_GLOBFUN(&CFcar, STACK(base, 9));
	Ffind(STACK(base, 4), 6);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 5));
		Fcdr(STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		COPY(STACK(base, 5), STACK(base, 0));
		COPY(STACK(base, 6), &mv_buf[0]);
		mv_count = 2;
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 0));
		COPY(STACK(base, 6), &mv_buf[0]);
		mv_count = 2;
	}
}
