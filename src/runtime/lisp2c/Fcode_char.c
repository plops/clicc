#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcode_char(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		LOAD_FIXNUM(0, STACK(base, 1));
		COPY(STACK(base, 0), STACK(base, 2));
		LOAD_FIXNUM(256, STACK(base, 3));
		Fle(STACK(base, 1), 3);
		bool_result = CL_TRUEP(STACK(base, 1));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		code_char_internal(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
