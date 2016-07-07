#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fwrite_to_string(CL_FORM *base, int nargs)
{
	BOOL supl_flags[10];
	static CL_FORM * keylist[] =
	{
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
	keysort(STACK(base, 1), nargs - 1, 10, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		COPY(SYMVAL(Slisp, 403), STACK(base, 1));	/* *PRINT-ESCAPE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 403), STACK(base, 1));	/* *PRINT-ESCAPE* */
	if(NOT(supl_flags[1]))
	{
		COPY(SYMVAL(Slisp, 408), STACK(base, 2));	/* *PRINT-RADIX* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 408), STACK(base, 2));	/* *PRINT-RADIX* */
	if(NOT(supl_flags[2]))
	{
		COPY(SYMVAL(Slisp, 400), STACK(base, 3));	/* *PRINT-BASE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 400), STACK(base, 3));	/* *PRINT-BASE* */
	if(NOT(supl_flags[3]))
	{
		COPY(SYMVAL(Slisp, 402), STACK(base, 4));	/* *PRINT-CIRCLE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 402), STACK(base, 4));	/* *PRINT-CIRCLE* */
	if(NOT(supl_flags[4]))
	{
		COPY(SYMVAL(Slisp, 407), STACK(base, 5));	/* *PRINT-PRETTY* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 407), STACK(base, 5));	/* *PRINT-PRETTY* */
	if(NOT(supl_flags[5]))
	{
		COPY(SYMVAL(Slisp, 406), STACK(base, 6));	/* *PRINT-LEVEL* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 406), STACK(base, 6));	/* *PRINT-LEVEL* */
	if(NOT(supl_flags[6]))
	{
		COPY(SYMVAL(Slisp, 405), STACK(base, 7));	/* *PRINT-LENGTH* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 405), STACK(base, 7));	/* *PRINT-LENGTH* */
	if(NOT(supl_flags[7]))
	{
		COPY(SYMVAL(Slisp, 401), STACK(base, 8));	/* *PRINT-CASE* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 401), STACK(base, 8));	/* *PRINT-CASE* */
	if(NOT(supl_flags[8]))
	{
		COPY(SYMVAL(Slisp, 404), STACK(base, 9));	/* *PRINT-GENSYM* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 404), STACK(base, 9));	/* *PRINT-GENSYM* */
	if(NOT(supl_flags[9]))
	{
		COPY(SYMVAL(Slisp, 399), STACK(base, 10));	/* *PRINT-ARRAY* */
	}
	BIND_SPECIAL(SYMBOL(Slisp, 399), STACK(base, 10));	/* *PRINT-ARRAY* */
	Fmake_string_output_stream(STACK(base, 11), 0);
	COPY(STACK(base, 0), STACK(base, 12));
	COPY(STACK(base, 11), STACK(base, 13));
	write2(STACK(base, 12));
	COPY(STACK(base, 11), STACK(base, 0));
	Fget_output_stream_string(STACK(base, 0));
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
