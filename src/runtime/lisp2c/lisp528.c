#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFFI_lisp_character)
{
	MAKE_STRING(47, "The evaluated value ~S is not of type c-<char>."),	/* 0 */
};

void FFI_lisp_character(CL_FORM *base)
{
	if(CL_C_CHAR_P(STACK(base, 0)))
	{
		LOAD_T(STACK(base, 1));
	}
	else
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		bool_result = CL_TRUEP(STACK(base, 1));
	}
	else
	{
		bool_result = CL_C_UNSIGNED_CHAR_P(STACK(base, 0));
	}
	if(bool_result)
	{
		make_lisp_character(STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFFI_lisp_character[0], STACK(base, 1));	/* The evaluated value ~S is not of type c-<char>. */
		COPY(STACK(base, 0), STACK(base, 2));
		Flist(STACK(base, 2), 1);
		LOAD_SMSTR((CL_FORM *)&KClisp[20], STACK(base, 3));	/* ~&Error in ~A: ~?~% */
		LOAD_SYMBOL(SYMBOL(Slisp, 605), STACK(base, 4));	/* LISP-CHARACTER */
		COPY(STACK(base, 1), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Ferror(STACK(base, 3), 4);
	}
}
