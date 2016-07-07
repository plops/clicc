#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void maptree(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 1)))
	{
		Ffuncall(STACK(base, 0), 2);
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		Fcar(STACK(base, 3));
		Ffuncall(STACK(base, 2), 2);
		mv_count = 1;
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Fcdr(STACK(base, 4));
		maptree(STACK(base, 3));
		mv_count = 1;
		COPY(STACK(base, 1), STACK(base, 4));
		Fcar(STACK(base, 4));
		if(EQ(STACK(base, 2), STACK(base, 4)) || CL_FLOATP(STACK(base, 2)) && GET_FLOAT(STACK(base, 2)) == GET_FLOAT(STACK(base, 4)))
		{
			COPY(STACK(base, 1), STACK(base, 4));
			Fcdr(STACK(base, 4));
			bool_result = EQ(STACK(base, 3), STACK(base, 4)) || CL_FLOATP(STACK(base, 3)) && GET_FLOAT(STACK(base, 3)) == GET_FLOAT(STACK(base, 4));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 1), STACK(base, 0));
		}
		else
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 4), 2);
			COPY(STACK(base, 2), CAR(lptr));
			COPY(STACK(base, 3), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
