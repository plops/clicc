#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Ftruncate(CL_FORM *base, int nargs)
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
	truncate_internal(STACK(base, 0));
}
