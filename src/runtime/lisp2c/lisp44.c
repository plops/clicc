#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void to_element_type(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 0)
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 1)
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 23), STACK(base, 0));	/* FIXNUM */
		}
		else
		{
			if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 2)
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 24), STACK(base, 0));	/* FLOAT */
			}
			else
			{
				if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 3)
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 0));	/* CHARACTER */
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
				}
			}
		}
	}
}
