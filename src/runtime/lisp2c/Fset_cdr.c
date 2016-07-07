#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_cdr = {Fset_cdr, 2};

void Fset_cdr(CL_FORM *base)
{
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
