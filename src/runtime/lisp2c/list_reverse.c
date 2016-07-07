#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void list_reverse(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 1));
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	M141_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN155;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 2), CAR(lptr));
		COPY(STACK(base, 1), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 1));
	}
	Fcdr(STACK(base, 3));
	goto M141_1;
	RETURN155:;
	COPY(STACK(base, 1), STACK(base, 0));
}
