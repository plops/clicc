#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flognand(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Flogand(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 3));
	if(CL_FIXNUMP(STACK(base, 2)))
	{
		COPY(STACK(base, 2), STACK(base, 3));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[8], STACK(base, 3));	/* type error: ~S is not of type ~S */
		COPY(STACK(base, 2), STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Slisp, 23), STACK(base, 5));	/* FIXNUM */
		Ferror(STACK(base, 3), 3);
	}
	LOAD_FIXNUM( ~ GET_FIXNUM(STACK(base, 3)), STACK(base, 0));
}
