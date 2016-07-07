#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fget_macro_character(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 382), STACK(base, 1));	/* *READTABLE* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 4));	/* READTABLE */
	struct_ref(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	Fchar_code(STACK(base, 3));
	vref(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 2), STACK(base, 3));
		Fcar(STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		Fcdr(STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 0));
		COPY(STACK(base, 4), &mv_buf[0]);
		mv_count = 2;
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
