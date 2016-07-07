#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fwrite(CL_FORM *base, int nargs)
{
	BOOL supl_flags[11];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 422),	/* STREAM */
		SYMBOL(Slisp, 423),	/* ESCAPE */
		SYMBOL(Slisp, 398),	/* RADIX */
		SYMBOL(Slisp, 424),	/* BASE */
		SYMBOL(Slisp, 425),	/* CIRCLE */
		SYMBOL(Slisp, 426),	/* PRETTY */
		SYMBOL(Slisp, 427),	/* LEVEL */
		SYMBOL(Slisp, 87),	/* LENGTH */
		SYMBOL(Slisp, 208),	/* CASE */
		SYMBOL(Slisp, 428),	/* GENSYM */
		SYMBOL(Slisp, 338),	/* ARRAY */
	};
	keysort(STACK(base, 1), nargs - 1, 11, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		COPY(SYMVAL(Slisp, 60), STACK(base, 1));	/* *STANDARD-OUTPUT* */
	}
	if(NOT(supl_flags[1]))
	{
		COPY(SYMVAL(Slisp, 403), STACK(base, 2));	/* *PRINT-ESCAPE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 403), STACK(base, 2));	/* *PRINT-ESCAPE* */
	if(NOT(supl_flags[2]))
	{
		COPY(SYMVAL(Slisp, 408), STACK(base, 3));	/* *PRINT-RADIX* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 408), STACK(base, 3));	/* *PRINT-RADIX* */
	if(NOT(supl_flags[3]))
	{
		COPY(SYMVAL(Slisp, 400), STACK(base, 4));	/* *PRINT-BASE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 400), STACK(base, 4));	/* *PRINT-BASE* */
	if(NOT(supl_flags[4]))
	{
		COPY(SYMVAL(Slisp, 402), STACK(base, 5));	/* *PRINT-CIRCLE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 402), STACK(base, 5));	/* *PRINT-CIRCLE* */
	if(NOT(supl_flags[5]))
	{
		COPY(SYMVAL(Slisp, 407), STACK(base, 6));	/* *PRINT-PRETTY* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 407), STACK(base, 6));	/* *PRINT-PRETTY* */
	if(NOT(supl_flags[6]))
	{
		COPY(SYMVAL(Slisp, 406), STACK(base, 7));	/* *PRINT-LEVEL* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 406), STACK(base, 7));	/* *PRINT-LEVEL* */
	if(NOT(supl_flags[7]))
	{
		COPY(SYMVAL(Slisp, 405), STACK(base, 8));	/* *PRINT-LENGTH* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 405), STACK(base, 8));	/* *PRINT-LENGTH* */
	if(NOT(supl_flags[8]))
	{
		COPY(SYMVAL(Slisp, 401), STACK(base, 9));	/* *PRINT-CASE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 401), STACK(base, 9));	/* *PRINT-CASE* */
	if(NOT(supl_flags[9]))
	{
		COPY(SYMVAL(Slisp, 404), STACK(base, 10));	/* *PRINT-GENSYM* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 404), STACK(base, 10));	/* *PRINT-GENSYM* */
	if(NOT(supl_flags[10]))
	{
		COPY(SYMVAL(Slisp, 399), STACK(base, 11));	/* *PRINT-ARRAY* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 399), STACK(base, 11));	/* *PRINT-ARRAY* */
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
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
}
