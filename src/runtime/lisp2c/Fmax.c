#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fmax(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	M40_1:;
	if(CL_ATOMP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 3));
		goto RETURN46;
	}
	COPY(STACK(base, 4), STACK(base, 5));
	Fcar(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 6));
	Fgt(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 3), STACK(base, 2));
	}
	Fcdr(STACK(base, 4));
	goto M40_1;
	RETURN46:;
	COPY(STACK(base, 2), STACK(base, 0));
}
