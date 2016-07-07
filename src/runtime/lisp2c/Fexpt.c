#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fexpt(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Fminusp(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_FIXNUM(1, STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Fminus(STACK(base, 4), 1);
		Fexpt(STACK(base, 3));
		Fdiv(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	else
	{
		expt_internal(STACK(base, 0));
	}
}
