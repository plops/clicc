#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flognot(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[8], STACK(base, 1));	/* type error: ~S is not of type ~S */
		COPY(STACK(base, 0), STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Slisp, 23), STACK(base, 3));	/* FIXNUM */
		Ferror(STACK(base, 1), 3);
	}
	LOAD_FIXNUM( ~ GET_FIXNUM(STACK(base, 1)), STACK(base, 0));
}
