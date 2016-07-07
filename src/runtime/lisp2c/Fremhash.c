#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fremhash(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	internal_sxhash(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 5));	/* HASH-TABLE */
	struct_ref(STACK(base, 3));
	Fmod(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	LOAD_FIXNUM(5, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 5));	/* HASH-TABLE */
	struct_ref(STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	LOAD_FIXNUM(5, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 8));	/* HASH-TABLE */
	struct_ref(STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	vref(STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 7));	/* TEST */
	COPY(STACK(base, 1), STACK(base, 8));
	LOAD_FIXNUM(4, STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 10));	/* HASH-TABLE */
	struct_ref(STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 9));	/* KEY */
	LOAD_GLOBFUN(&CFcar, STACK(base, 10));
	Fdelete(STACK(base, 5), 6);
	COPY(STACK(base, 5), STACK(base, 0));
	COPY(STACK(base, 3), STACK(base, 1));
	COPY(STACK(base, 4), STACK(base, 2));
	Fset_aref(STACK(base, 0), 3);
}
