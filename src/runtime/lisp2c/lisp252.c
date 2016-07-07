#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFset_dispatch_macro_character)
{
	MAKE_STRING(36, "~S is not a dispatch macro character"),	/* 0 */
};

void Fset_dispatch_macro_character(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 3:
		COPY(SYMVAL(Slisp, 382), STACK(base, 3));	/* *READTABLE* */
		case 4:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	LOAD_FIXNUM(1, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 7));	/* READTABLE */
	struct_ref(STACK(base, 5));
	Fassoc(STACK(base, 4), 2);
	Fcdr(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFset_dispatch_macro_character[0], STACK(base, 5));	/* ~S is not a dispatch macro character */
		COPY(STACK(base, 0), STACK(base, 6));
		Ferror(STACK(base, 5), 2);
	}
	COPY(STACK(base, 1), STACK(base, 5));
	Fchar_upcase(STACK(base, 5));
	Fchar_code(STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Fset_aref(STACK(base, 6), 3);
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
}
