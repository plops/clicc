#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cconcatenate_to_list = {concatenate_to_list, -1};

void concatenate_to_list(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	LOAD_NIL(STACK(base, 1));
	LOAD_NIL(STACK(base, 2));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 3), 2);
		COPY(STACK(base, 1), CAR(lptr));
		COPY(STACK(base, 2), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 1));
	}
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	M148_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN162;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	if(CL_LISTP(STACK(base, 2)))
	{
		LOAD_NIL(STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		M149_1:;
		if(CL_ATOMP(STACK(base, 5)))
		{
			LOAD_NIL(STACK(base, 4));
			goto RETURN163;
		}
		COPY(STACK(base, 5), STACK(base, 6));
		Fcar(STACK(base, 6));
		COPY(STACK(base, 6), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 7));
		add_q(STACK(base, 6));
		Fcdr(STACK(base, 5));
		goto M149_1;
		RETURN163:;
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 4));
		Flength(STACK(base, 4));
		LOAD_FIXNUM(0, STACK(base, 5));
		M150_1:;
		COPY(STACK(base, 5), STACK(base, 6));
		COPY(STACK(base, 4), STACK(base, 7));
		Fge(STACK(base, 6), 2);
		if(CL_TRUEP(STACK(base, 6)))
		{
			goto RETURN164;
		}
		COPY(STACK(base, 2), STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 7));
		Felt(STACK(base, 6));
		COPY(STACK(base, 1), STACK(base, 7));
		add_q(STACK(base, 6));
		F1plus(STACK(base, 5));
		goto M150_1;
		RETURN164:;
	}
	Fcdr(STACK(base, 3));
	goto M148_1;
	RETURN162:;
	COPY(STACK(base, 1), STACK(base, 0));
	Fcar(STACK(base, 0));
}
