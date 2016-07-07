#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fnbutlast(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_FIXNUM(1, STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	Flength(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fminus(STACK(base, 2), 2);
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	LOAD_FIXNUM(0, STACK(base, 5));
	Fle(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 3));
		F1minus(STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Fnthcdr(STACK(base, 3));
		if(CL_CONSP(STACK(base, 3)))
		{
			LOAD_NIL(STACK(base, 4));
			COPY(STACK(base, 4), CDR(GET_FORM(STACK(base, 3))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[2], STACK(base, 4));	/* ~a is not a cons */
			COPY(STACK(base, 3), STACK(base, 5));
			Ferror(STACK(base, 4), 2);
		}
	}
}
