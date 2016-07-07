#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFconcatenate = {Fconcatenate, -2};

void Fconcatenate(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 28))	/* LIST */
	{
		LOAD_GLOBFUN(&Cconcatenate_to_list, STACK(base, 0));
		Fapply(STACK(base, 0), 2);
	}
	else
	{
		LOAD_GLOBFUN(&Cconcatenate_to_non_list, STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Fapply(STACK(base, 2), 3);
		COPY(STACK(base, 2), STACK(base, 0));
	}
}
