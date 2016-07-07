#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_caadar(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Fcar(STACK(base, 2));
	Fcdr(STACK(base, 2));
	Fcar(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	if(CL_CONSP(STACK(base, 2)))
	{
		COPY(STACK(base, 0), CAR(GET_FORM(STACK(base, 2))));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[12], STACK(base, 0));	/* ~a is not a cons */
		COPY(STACK(base, 2), STACK(base, 1));
		Ferror(STACK(base, 0), 2);
	}
}
