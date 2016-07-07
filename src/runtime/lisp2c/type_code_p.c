#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void type_code_p(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 1)) && GET_FIXNUM(STACK(base, 1)) == 0)
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		if(CL_FIXNUMP(STACK(base, 1)) && GET_FIXNUM(STACK(base, 1)) == 1)
		{
			if(CL_FIXNUMP(STACK(base, 0)))
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
			if(CL_FIXNUMP(STACK(base, 1)) && GET_FIXNUM(STACK(base, 1)) == 2)
			{
				if(CL_FLOATP(STACK(base, 0)))
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
				if(CL_FIXNUMP(STACK(base, 1)) && GET_FIXNUM(STACK(base, 1)) == 3)
				{
					if(CL_CHARP(STACK(base, 0)))
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
	}
}
