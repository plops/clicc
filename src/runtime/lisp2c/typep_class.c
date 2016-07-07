#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void typep_class(CL_FORM *base)
{
	if(CL_INSTANCEP(STACK(base, 0)))
	{
		LOAD_FIXNUM(-1, STACK(base, 2));
		COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 2))), STACK(base, 2));
		if(EQ(STACK(base, 1), STACK(base, 2)))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
		}
		else
		{
			LOAD_FIXNUM(-1, STACK(base, 2));
			COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 2))), STACK(base, 2));
			LOAD_FIXNUM(1, STACK(base, 3));
			COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 2))), 1 + GET_FIXNUM(STACK(base, 3))), STACK(base, 2));
			M13_1:;
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(STACK(base, 2), STACK(base, 3));
				Fcar(STACK(base, 3));
				if(EQ(STACK(base, 1), STACK(base, 3)))
				{
					COPY(STACK(base, 1), STACK(base, 0));
				}
				else
				{
					Fcdr(STACK(base, 2));
					goto M13_1;
				}
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
			goto RETURN13;
			RETURN13:;
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
