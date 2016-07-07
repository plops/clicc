#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fprint(CL_FORM *base, int nargs)
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
	COPY(STACK(base, 1), STACK(base, 3));
	Fterpri(STACK(base, 3), 1);
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	write2(STACK(base, 3));
	LOAD_CHAR(' ', STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Fwrite_char(STACK(base, 3), 2);
	mv_count = 1;
	RESTORE_SPECIAL;
}
