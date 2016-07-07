#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fbutlast(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_FIXNUM(1, STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	Flength(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fminus(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	Fle(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		LOAD_FIXNUM(0, STACK(base, 1));
		Fsubseq(STACK(base, 0), 3);
	}
}
