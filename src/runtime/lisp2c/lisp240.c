#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cnull_or_quoted = {null_or_quoted, 1};

void null_or_quoted(CL_FORM *base)
{
	if(NOT(CL_TRUEP(STACK(base, 0))))
	{
		LOAD_T(STACK(base, 1));
	}
	else
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
			if(EQ(STACK(base, 2), SYMVAL(Slisp, 372)))	/* *BQ-QUOTE* */
			{
				LOAD_T(STACK(base, 0));
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
}
