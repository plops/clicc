#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fldiff(CL_FORM *base)
{
	if(NOT(CL_TRUEP(STACK(base, 0))))
	{
		LOAD_T(STACK(base, 2));
	}
	else
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(CL_TRUEP(STACK(base, 2)))
	{
		bool_result = TRUE;
	}
	else
	{
		bool_result = EQ(STACK(base, 0), STACK(base, 1));
	}
	if(bool_result)
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		Fcdr(STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Fldiff(STACK(base, 3));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 4), 2);
			COPY(STACK(base, 2), CAR(lptr));
			COPY(STACK(base, 3), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
