#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void copy_dispatch_macro_character(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	Fcar(STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	Fcdr(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	COPY(STACK(base, 1), STACK(base, 7));
	LOAD_FIXNUM(0, STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 9));	/* READTABLE */
	struct_ref(STACK(base, 7));
	COPY(STACK(base, 3), STACK(base, 8));
	Fchar_code(STACK(base, 8));
	vref(STACK(base, 7));
	Fcdr(STACK(base, 7));
	COPY(STACK(base, 2), STACK(base, 8));
	Fmake_dispatch_macro_character(STACK(base, 6), 3);
	COPY(STACK(base, 3), STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	LOAD_FIXNUM(1, STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 9));	/* READTABLE */
	struct_ref(STACK(base, 7));
	Fassoc(STACK(base, 6), 2);
	Fcdr(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 5));
	LOAD_FIXNUM(256, STACK(base, 6));
	LOAD_FIXNUM(0, STACK(base, 7));
	M86_1:;
	COPY(STACK(base, 7), STACK(base, 8));
	COPY(STACK(base, 6), STACK(base, 9));
	Fge(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN95;
	}
	COPY(STACK(base, 4), STACK(base, 8));
	COPY(STACK(base, 7), STACK(base, 9));
	vref(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 9));
	COPY(STACK(base, 5), STACK(base, 10));
	COPY(STACK(base, 7), STACK(base, 11));
	Fset_aref(STACK(base, 9), 3);
	F1plus(STACK(base, 7));
	goto M86_1;
	RETURN95:;
}
