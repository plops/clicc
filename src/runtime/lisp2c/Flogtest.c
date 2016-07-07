#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flogtest(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Flogand(STACK(base, 2), 2);
	Fzerop(STACK(base, 2));
	if(NOT(CL_TRUEP(STACK(base, 2))))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
