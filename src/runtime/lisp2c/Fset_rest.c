#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_rest(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	if(CL_CONSP(STACK(base, 1)))
	{
		COPY(STACK(base, 0), CDR(GET_FORM(STACK(base, 1))));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[6], STACK(base, 0));	/* ~a is not a cons */
		Ferror(STACK(base, 0), 2);
	}
}
