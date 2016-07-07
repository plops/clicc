#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFwrite_string = {Fwrite_string, -2};

void Fwrite_string(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		nargs = 2;
	}
	{
		BOOL supl_flags[2];
		static CL_FORM * keylist[] =
		{
			SYMBOL(Slisp, 161),	/* START */
			SYMBOL(Slisp, 162),	/* END */
		};
		keysort(STACK(base, 2), nargs - 2, 2, keylist, supl_flags, FALSE);
		if(NOT(supl_flags[0]))
		{
			LOAD_FIXNUM(0, STACK(base, 2));
		}
		if(NOT(supl_flags[1]))
		{
			COPY(STACK(base, 0), STACK(base, 4));
			Flength(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 3));
		}
	}
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
	M117_1:;
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	Fge(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		goto RETURN130;
	}
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fchar(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	Fwrite_char(STACK(base, 4), 2);
	mv_count = 1;
	COPY(STACK(base, 2), STACK(base, 4));
	F1plus(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	goto M117_1;
	RETURN130:;
}
