#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flogorc2(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 4));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[8], STACK(base, 4));	/* type error: ~S is not of type ~S */
		COPY(STACK(base, 1), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Slisp, 23), STACK(base, 6));	/* FIXNUM */
		Ferror(STACK(base, 4), 3);
	}
	LOAD_FIXNUM( ~ GET_FIXNUM(STACK(base, 4)), STACK(base, 3));
	Flogior(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}
