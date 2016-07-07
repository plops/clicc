#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_char(CL_FORM *base)
{
	if(CL_STRING_P(STACK(base, 1)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 57), STACK(base, 3));	/* WRONG_TYPE */
		LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 4));	/* STRING */
		COPY(STACK(base, 1), STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	if(CL_CHARP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 57), STACK(base, 3));	/* WRONG_TYPE */
		LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 4));	/* CHARACTER */
		COPY(STACK(base, 0), STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	if(CL_FIXNUMP(STACK(base, 2)))
	{
		LOAD_FIXNUM(0, STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Flength(STACK(base, 5));
		F1minus(STACK(base, 5));
		Fle(STACK(base, 3), 3);
		bool_result = CL_TRUEP(STACK(base, 3));
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
		COPY(SYMVAL(Slisp, 583), STACK(base, 3));	/* NO_INDEX */
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		LOAD_FIXNUM(0, STACK(base, 6));
		COPY(STACK(base, 1), STACK(base, 7));
		Flength(STACK(base, 7));
		Ferror(STACK(base, 3), 5);
	}
	set_char_internal(STACK(base, 0));
}
