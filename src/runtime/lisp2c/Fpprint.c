#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fpprint(CL_FORM *base, int nargs)
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
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
	BIND_SPECIAL(SYMBOL(Slisp, 403), STACK(base, 2));	/* *PRINT-ESCAPE* */
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
	BIND_SPECIAL(SYMBOL(Slisp, 407), STACK(base, 3));	/* *PRINT-PRETTY* */
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
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	write2(STACK(base, 4));
	LOAD_NIL(STACK(base, 0));
	mv_count = 0;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
}
