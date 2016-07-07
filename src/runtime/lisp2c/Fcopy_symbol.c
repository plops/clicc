#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcopy_symbol(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	Fsymbol_name(STACK(base, 2));
	Fmake_symbol(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 0), STACK(base, 3));
		Fboundp(STACK(base, 3));
		if(CL_TRUEP(STACK(base, 3)))
		{
			COPY(STACK(base, 2), STACK(base, 3));
			COPY(STACK(base, 0), STACK(base, 4));
			Fsymbol_value(STACK(base, 4));
			Fset(STACK(base, 3));
		}
		COPY(STACK(base, 0), STACK(base, 3));
		Fsymbol_plist(STACK(base, 3));
		Fcopy_list(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fset_symbol_plist(STACK(base, 4));
	}
	COPY(STACK(base, 2), STACK(base, 0));
}
