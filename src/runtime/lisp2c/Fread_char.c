#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFread_char)
{
	MAKE_STRING(24, "unexpected end-of-stream"),	/* 0 */
};

void Fread_char(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		LOAD_NIL(STACK(base, 0));
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
	COPY(STACK(base, 0), STACK(base, 4));
	stream_readc(STACK(base, 4));
	Ffuncall(STACK(base, 4), 1);
	mv_count = 1;
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 0));
	}
	else
	{
		if(CL_TRUEP(STACK(base, 1)))
		{
			bool_result = CL_TRUEP(STACK(base, 1));
		}
		else
		{
			bool_result = CL_TRUEP(STACK(base, 3));
		}
		if(bool_result)
		{
			LOAD_SMSTR((CL_FORM *)&KFread_char[0], STACK(base, 5));	/* unexpected end-of-stream */
			Ferror(STACK(base, 5), 1);
		}
		COPY(STACK(base, 2), STACK(base, 0));
	}
}
