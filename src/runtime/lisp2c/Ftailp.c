#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Ftailp(CL_FORM *base)
{
	M198_1:;
	if(EQ(STACK(base, 0), STACK(base, 1)) || CL_FLOATP(STACK(base, 0)) && GET_FLOAT(STACK(base, 0)) == GET_FLOAT(STACK(base, 1)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		if(CL_ATOMP(STACK(base, 1)))
		{
			LOAD_NIL(STACK(base, 0));
		}
		else
		{
			Fcdr(STACK(base, 1));
			goto M198_1;
		}
	}
	goto RETURN217;
	RETURN217:;
}
