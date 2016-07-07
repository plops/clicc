#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFpairlis)
{
	MAKE_STRING(49, "The lists of keys and data are of unequal length."),	/* 0 */
};

void Fpairlis(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	M202_1:;
	if(CL_ATOMP(STACK(base, 0)))
	{
		if(CL_ATOMP(STACK(base, 1)))
		{
			COPY(STACK(base, 2), STACK(base, 0));
			goto RETURN221;
		}
		else
		{
			goto M202_2;
		}
	}
	else
	{
		if(CL_ATOMP(STACK(base, 1)))
		{
			goto M202_2;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 3));
			Fcar(STACK(base, 3));
			COPY(STACK(base, 1), STACK(base, 4));
			Fcar(STACK(base, 4));
			COPY(STACK(base, 2), STACK(base, 5));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 6), 2);
				COPY(STACK(base, 3), CAR(lptr));
				COPY(STACK(base, 4), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 6));
			}
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 6), CAR(lptr));
				COPY(STACK(base, 5), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 2));
			}
			COPY(STACK(base, 0), STACK(base, 3));
			COPY(STACK(base, 3), STACK(base, 4));
			Fcdr(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 0));
			COPY(STACK(base, 1), STACK(base, 3));
			COPY(STACK(base, 3), STACK(base, 4));
			Fcdr(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 1));
		}
	}
	goto M202_1;
	M202_2:;
	LOAD_SMSTR((CL_FORM *)&KFpairlis[0], STACK(base, 3));	/* The lists of keys and data are of unequal length. */
	Ferror(STACK(base, 3), 1);
	LOAD_NIL(STACK(base, 0));
	RETURN221:;
}
