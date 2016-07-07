#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_get(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 3:
		LOAD_NIL(STACK(base, 3));
		case 4:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 1), STACK(base, 4));
	Fsymbol_plist(STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	set_prop(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	Fset_symbol_plist(STACK(base, 5));
}
