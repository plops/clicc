#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fread(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		COPY(SYMVAL(Slisp, 59), STACK(base, 0));	/* *STANDARD-INPUT* */
		case 1:
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		LOAD_NIL(STACK(base, 3));
		case 4:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Ssys, 1))	/* T */
		{
			COPY(SYMVAL(Slisp, 58), STACK(base, 0));	/* *TERMINAL-IO* */
		}
	}
	else
	{
		COPY(SYMVAL(Slisp, 59), STACK(base, 0));	/* *STANDARD-INPUT* */
	}
	LOAD_FIXNUM(0, STACK(base, 4));
	BIND_SPECIAL(SYMBOL(Slisp, 375), STACK(base, 4));	/* *BQ-LEVEL* */
	if(CL_TRUEP(STACK(base, 3)))
	{
		internal_read(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 5));
		BIND_SPECIAL(SYMBOL(Slisp, 378), STACK(base, 5));	/* *PRESERVE-WHITESPACE* */
		LOAD_NIL(STACK(base, 3));
		internal_read(STACK(base, 0));
		RESTORE_SPECIAL;
	}
	RESTORE_SPECIAL;
}
