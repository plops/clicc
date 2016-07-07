#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fash(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Fminusp(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), STACK(base, 2));
		Fminus(STACK(base, 2), 1);
		LOAD_FIXNUM(GET_FIXNUM(STACK(base, 0)) >> GET_FIXNUM(STACK(base, 2)), STACK(base, 0));
	}
	else
	{
		LOAD_FIXNUM(GET_FIXNUM(STACK(base, 0)) <<  GET_FIXNUM(STACK(base, 1)), STACK(base, 0));
	}
}
