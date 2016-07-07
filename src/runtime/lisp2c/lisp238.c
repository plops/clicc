#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cbq_splicing_frob = {bq_splicing_frob, 1};

void bq_splicing_frob(CL_FORM *base)
{
	if(CL_CONSP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(EQ(STACK(base, 1), SYMVAL(Slisp, 365)))	/* *COMMA-ATSIGN* */
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
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
			if(EQ(STACK(base, 2), SYMVAL(Slisp, 366)))	/* *COMMA-DOT* */
			{
				LOAD_T(STACK(base, 0));
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
