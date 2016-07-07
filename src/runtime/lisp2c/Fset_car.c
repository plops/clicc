#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_car = {Fset_car, 2};

void Fset_car(CL_FORM *base)
{
	if(CL_CONSP(STACK(base, 1)))
	{
		COPY(STACK(base, 0), CAR(GET_FORM(STACK(base, 1))));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[12], STACK(base, 0));	/* ~a is not a cons */
		Ferror(STACK(base, 0), 2);
	}
}
