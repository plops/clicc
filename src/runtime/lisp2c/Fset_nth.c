#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_nth = {Fset_nth, 3};

void Fset_nth(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	Fnthcdr(STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	if(CL_CONSP(STACK(base, 3)))
	{
		COPY(STACK(base, 0), CAR(GET_FORM(STACK(base, 3))));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[12], STACK(base, 0));	/* ~a is not a cons */
		COPY(STACK(base, 3), STACK(base, 1));
		Ferror(STACK(base, 0), 2);
	}
}
