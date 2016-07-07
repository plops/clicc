#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void simple_member(CL_FORM *base)
{
	M13_1:;
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 2));
		Fcar(STACK(base, 2));
		if(EQ(STACK(base, 0), STACK(base, 2)))
		{
		}
		else
		{
			Fcdr(STACK(base, 1));
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
