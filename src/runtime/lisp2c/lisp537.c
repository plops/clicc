#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fget_properties(CL_FORM *base)
{
	M241_1:;
	if(CL_ATOMP(STACK(base, 0)))
	{
		LOAD_NIL(STACK(base, 2));
		LOAD_NIL(STACK(base, 3));
		LOAD_NIL(STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 0));
		COPY(STACK(base, 3), &mv_buf[0]);
		COPY(STACK(base, 4), &mv_buf[1]);
		mv_count = 3;
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		Fmember(STACK(base, 2), 2);
		if(CL_TRUEP(STACK(base, 2)))
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			Fcdr(STACK(base, 3));
			Fcar(STACK(base, 3));
			COPY(STACK(base, 0), STACK(base, 4));
			COPY(STACK(base, 2), STACK(base, 0));
			COPY(STACK(base, 3), &mv_buf[0]);
			COPY(STACK(base, 4), &mv_buf[1]);
			mv_count = 3;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 2));
			COPY(STACK(base, 2), STACK(base, 3));
			Fcdr(STACK(base, 3));
			Fcdr(STACK(base, 3));
			COPY(STACK(base, 3), STACK(base, 0));
			goto M241_1;
		}
	}
	goto RETURN282;
	RETURN282:;
}
