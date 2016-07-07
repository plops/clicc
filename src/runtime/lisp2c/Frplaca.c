#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Frplaca(CL_FORM *base)
{
	if(CL_CONSP(STACK(base, 0)))
	{
		COPY(STACK(base, 1), CAR(GET_FORM(STACK(base, 0))));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[0], STACK(base, 2));	/* ~a is not a cons */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
}
