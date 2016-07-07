#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fprinc(CL_FORM *base, int nargs)
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
	LOAD_NIL(STACK(base, 2));
	BIND_SPECIAL(SYMBOL(Slisp, 403), STACK(base, 2));	/* *PRINT-ESCAPE* */
	if(CL_TRUEP(STACK(base, 1)))
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Ssys, 1))	/* T */
		{
			COPY(SYMVAL(Slisp, 58), STACK(base, 1));	/* *TERMINAL-IO* */
		}
	}
	else
	{
		COPY(SYMVAL(Slisp, 60), STACK(base, 1));	/* *STANDARD-OUTPUT* */
	}
	write2(STACK(base, 0));
	RESTORE_SPECIAL;
}
