#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fget(CL_FORM *base, int nargs)
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
	Fsymbol_plist(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	get_prop(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 0));
		Fcdr(STACK(base, 0));
		Fcar(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 0));
	}
}
