#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fnotany(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 2), nargs - 2);
	LOAD_GLOBFUN(&CFsome, STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	Fapply(STACK(base, 3), 4);
	mv_count = 1;
	if(NOT(CL_TRUEP(STACK(base, 3))))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
