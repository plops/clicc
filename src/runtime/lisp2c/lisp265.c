#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFget_dispatch_macro_character)
{
	MAKE_STRING(36, "~S is not a dispatch macro character"),	/* 0 */
};

void Fget_dispatch_macro_character(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		COPY(SYMVAL(Slisp, 382), STACK(base, 2));	/* *READTABLE* */
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	LOAD_FIXNUM(1, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 6));	/* READTABLE */
	struct_ref(STACK(base, 4));
	Fassoc(STACK(base, 3), 2);
	Fcdr(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFget_dispatch_macro_character[0], STACK(base, 4));	/* ~S is not a dispatch macro character */
		COPY(STACK(base, 0), STACK(base, 5));
		Ferror(STACK(base, 4), 2);
	}
	COPY(STACK(base, 3), STACK(base, 0));
	Fchar_upcase(STACK(base, 1));
	Fchar_code(STACK(base, 1));
	vref(STACK(base, 0));
}
