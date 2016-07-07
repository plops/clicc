#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fschar(CL_FORM *base)
{
	if(CL_SMSTRP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 57), STACK(base, 2));	/* WRONG_TYPE */
		LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 3));	/* SIMPLE-STRING */
		COPY(STACK(base, 0), STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		LOAD_FIXNUM(0, STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Flength(STACK(base, 4));
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
		COPY(SYMVAL(Slisp, 583), STACK(base, 2));	/* NO_INDEX */
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		LOAD_FIXNUM(0, STACK(base, 5));
		COPY(STACK(base, 0), STACK(base, 6));
		Flength(STACK(base, 6));
		Ferror(STACK(base, 2), 5);
	}
	schar_internal(STACK(base, 0));
}
