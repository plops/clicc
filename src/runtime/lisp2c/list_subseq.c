#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Klist_subseq)
{
	MAKE_STRING(23, "end of sequence reached"),	/* 0 */
};

void list_subseq(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 5), 2);
		COPY(STACK(base, 3), CAR(lptr));
		COPY(STACK(base, 4), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 3));
	}
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fnthcdr(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 0));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Fminus(STACK(base, 4), 2);
		COPY(STACK(base, 4), STACK(base, 2));
		LOAD_FIXNUM(0, STACK(base, 4));
		M138_1:;
		COPY(STACK(base, 4), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Fge(STACK(base, 5), 2);
		if(CL_TRUEP(STACK(base, 5)))
		{
			goto RETURN152;
		}
		if(CL_ATOMP(STACK(base, 0)))
		{
			LOAD_SMSTR((CL_FORM *)&Klist_subseq[0], STACK(base, 5));	/* end of sequence reached */
			Ferror(STACK(base, 5), 1);
		}
		COPY(STACK(base, 0), STACK(base, 5));
		Fcar(STACK(base, 5));
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 6), STACK(base, 7));
		Fcdr(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 0));
		COPY(STACK(base, 3), STACK(base, 6));
		add_q(STACK(base, 5));
		F1plus(STACK(base, 4));
		goto M138_1;
		RETURN152:;
	}
	else
	{
		M139_1:;
		if(CL_TRUEP(STACK(base, 0)))
		{
		}
		else
		{
			goto RETURN153;
		}
		COPY(STACK(base, 0), STACK(base, 4));
		Fcar(STACK(base, 4));
		COPY(STACK(base, 0), STACK(base, 5));
		COPY(STACK(base, 5), STACK(base, 6));
		Fcdr(STACK(base, 6));
		COPY(STACK(base, 6), STACK(base, 0));
		COPY(STACK(base, 3), STACK(base, 5));
		add_q(STACK(base, 4));
		goto M139_1;
		RETURN153:;
	}
	COPY(STACK(base, 3), STACK(base, 0));
	Fcar(STACK(base, 0));
}
