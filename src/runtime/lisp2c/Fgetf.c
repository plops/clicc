#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fgetf(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	get_prop(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 3), STACK(base, 0));
		Fcdr(STACK(base, 0));
		Fcar(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 0));
	}
}
