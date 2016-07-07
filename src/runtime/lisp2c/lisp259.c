#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_syntax_from_char(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		COPY(SYMVAL(Slisp, 382), STACK(base, 2));	/* *READTABLE* */
		case 3:
		COPY(SYMVAL(Slisp, 381), STACK(base, 3));	/* *STANDARD-READTABLE* */
		case 4:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	LOAD_FIXNUM(0, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 7));	/* READTABLE */
	struct_ref(STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	Fchar_code(STACK(base, 6));
	COPY(STACK(base, 3), STACK(base, 7));
	LOAD_FIXNUM(0, STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 9));	/* READTABLE */
	struct_ref(STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	Fchar_code(STACK(base, 8));
	vref(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 8));
	COPY(STACK(base, 5), STACK(base, 9));
	COPY(STACK(base, 6), STACK(base, 10));
	Fset_aref(STACK(base, 8), 3);
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	LOAD_FIXNUM(1, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 8));	/* READTABLE */
	struct_ref(STACK(base, 6));
	Fassoc(STACK(base, 5), 2);
	COPY(STACK(base, 5), STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 5));
		if(CL_CONSP(STACK(base, 4)))
		{
			LOAD_NIL(STACK(base, 6));
			COPY(STACK(base, 6), CAR(GET_FORM(STACK(base, 4))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[12], STACK(base, 6));	/* ~a is not a cons */
			COPY(STACK(base, 4), STACK(base, 7));
			Ferror(STACK(base, 6), 2);
		}
		COPY(STACK(base, 4), STACK(base, 5));
		if(CL_CONSP(STACK(base, 4)))
		{
			LOAD_NIL(STACK(base, 6));
			COPY(STACK(base, 6), CDR(GET_FORM(STACK(base, 4))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[6], STACK(base, 6));	/* ~a is not a cons */
			COPY(STACK(base, 4), STACK(base, 7));
			Ferror(STACK(base, 6), 2);
		}
	}
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	LOAD_FIXNUM(1, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 8));	/* READTABLE */
	struct_ref(STACK(base, 6));
	Fassoc(STACK(base, 5), 2);
	COPY(STACK(base, 5), STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 5));
		COPY(STACK(base, 3), STACK(base, 6));
		COPY(STACK(base, 2), STACK(base, 7));
		copy_dispatch_macro_character(STACK(base, 5));
	}
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
}
