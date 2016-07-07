#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fnthcdr(CL_FORM *base)
{
	M195_1:;
	COPY(STACK(base, 0), STACK(base, 2));
	Fzerop(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
		goto RETURN214;
	}
	else
	{
		if(CL_TRUEP(STACK(base, 1)))
		{
			COPY(STACK(base, 0), STACK(base, 2));
			F1minus(STACK(base, 2));
			COPY(STACK(base, 2), STACK(base, 0));
			COPY(STACK(base, 1), STACK(base, 2));
			COPY(STACK(base, 2), STACK(base, 1));
			Fcdr(STACK(base, 1));
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN214;
		}
	}
	goto M195_1;
	RETURN214:;
}
