#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flist_length(CL_FORM *base)
{
	LOAD_FIXNUM(0, STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	M194_1:;
	COPY(STACK(base, 2), STACK(base, 4));
	Fendp(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
		goto RETURN213;
	}
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 5));
	Fcdr(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 4));
	Fendp(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
		F1plus(STACK(base, 0));
		goto RETURN213;
	}
	if(EQ(STACK(base, 2), STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN213;
	}
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(2, STACK(base, 5));
	Fplus(STACK(base, 4), 2);
	COPY(STACK(base, 2), STACK(base, 5));
	Fcdr(STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fcdr(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 3));
	COPY(STACK(base, 5), STACK(base, 2));
	COPY(STACK(base, 4), STACK(base, 1));
	goto M194_1;
	RETURN213:;
}
