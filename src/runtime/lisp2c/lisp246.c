#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcopy_readtable(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		COPY(SYMVAL(Slisp, 382), STACK(base, 0));	/* *READTABLE* */
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 381), STACK(base, 0));	/* *STANDARD-READTABLE* */
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
	}
	else
	{
		LOAD_FIXNUM(256, STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 3));	/* INITIAL-ELEMENT */
		LOAD_NIL(STACK(base, 4));
		Fmake_array(STACK(base, 2), 3);
		LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 1));	/* READTABLE */
		LOAD_NIL(STACK(base, 3));
		make_struct(STACK(base, 1), 3);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 4));	/* READTABLE */
	struct_ref(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 5));	/* READTABLE */
	struct_ref(STACK(base, 3));
	LOAD_FIXNUM(256, STACK(base, 4));
	LOAD_FIXNUM(0, STACK(base, 5));
	M84_1:;
	COPY(STACK(base, 5), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fge(STACK(base, 6), 2);
	if(CL_TRUEP(STACK(base, 6)))
	{
		goto RETURN93;
	}
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 5), STACK(base, 7));
	vref(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 7));
	COPY(STACK(base, 3), STACK(base, 8));
	COPY(STACK(base, 5), STACK(base, 9));
	Fset_aref(STACK(base, 7), 3);
	F1plus(STACK(base, 5));
	goto M84_1;
	RETURN93:;
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	LOAD_FIXNUM(1, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 7));	/* READTABLE */
	LOAD_NIL(STACK(base, 8));
	set_struct_ref(STACK(base, 5));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	LOAD_FIXNUM(1, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 7));	/* READTABLE */
	struct_ref(STACK(base, 5));
	M85_1:;
	if(CL_ATOMP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 4));
		goto RETURN94;
	}
	COPY(STACK(base, 5), STACK(base, 6));
	Fcar(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 4));
	Fcar(STACK(base, 6));
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 4), STACK(base, 6));
		COPY(STACK(base, 0), STACK(base, 7));
		COPY(STACK(base, 1), STACK(base, 8));
		copy_dispatch_macro_character(STACK(base, 6));
	}
	Fcdr(STACK(base, 5));
	goto M85_1;
	RETURN94:;
	COPY(STACK(base, 1), STACK(base, 0));
}
