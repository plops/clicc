#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_string)
{
	MAKE_STRING(36, "~S is not a legal size for a string."),	/* 0 */
	MAKE_STRING(56, "The value of INITIAL-ELEMENT, ~S, should be a CHARACTER."),	/* 2 */
};

void Fmake_string(CL_FORM *base, int nargs)
{
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 123),	/* INITIAL-ELEMENT */
	};
	keysort(STACK(base, 1), nargs - 1, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_CHAR(' ', STACK(base, 1));
	}
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		LOAD_FIXNUM(0, STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		LOAD_FIXNUM(2147483647, STACK(base, 4));
		F1minus(STACK(base, 4));
		Fle(STACK(base, 2), 3);
		bool_result = CL_TRUEP(STACK(base, 2));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_string[0], STACK(base, 2));	/* ~S is not a legal size for a string. */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	if(CL_CHARP(STACK(base, 1)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_string[2], STACK(base, 2));	/* The value of INITIAL-ELEMENT, ~S, should be a CHARACTER. */
		COPY(STACK(base, 1), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	make_string_internal(STACK(base, 0));
}
