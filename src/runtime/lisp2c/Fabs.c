#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fabs(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Fminusp(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
		Fminus(STACK(base, 0), 1);
	}
	else
	{
	}
}
