#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fdigit_char(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_FIXNUM(10, STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		LOAD_FIXNUM(2, STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		LOAD_FIXNUM(36, STACK(base, 4));
		Fle(STACK(base, 2), 3);
		bool_result = CL_TRUEP(STACK(base, 2));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		if(CL_FIXNUMP(STACK(base, 0)))
		{
			LOAD_FIXNUM(0, STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			COPY(STACK(base, 1), STACK(base, 4));
			F1minus(STACK(base, 4));
			Fle(STACK(base, 2), 3);
			bool_result = CL_TRUEP(STACK(base, 2));
		}
		else
		{
			bool_result = FALSE;
		}
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		digit_char_internal(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
