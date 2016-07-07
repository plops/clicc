#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void get_prop(CL_FORM *base)
{
	M239_1:;
	if(CL_ATOMP(STACK(base, 0)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fcar(STACK(base, 2));
		if(EQ(STACK(base, 2), STACK(base, 1)))
		{
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 2));
			COPY(STACK(base, 2), STACK(base, 3));
			Fcdr(STACK(base, 3));
			Fcdr(STACK(base, 3));
			COPY(STACK(base, 3), STACK(base, 0));
			goto M239_1;
		}
	}
	goto RETURN279;
	RETURN279:;
}
