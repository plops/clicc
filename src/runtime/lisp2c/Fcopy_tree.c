#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcopy_tree(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		Fcopy_tree(STACK(base, 1));
		COPY(STACK(base, 0), STACK(base, 2));
		Fcdr(STACK(base, 2));
		Fcopy_tree(STACK(base, 2));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 3), 2);
			COPY(STACK(base, 1), CAR(lptr));
			COPY(STACK(base, 2), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
