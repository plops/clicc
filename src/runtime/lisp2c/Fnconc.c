#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFnconc = {Fnconc, -1};

void Fnconc(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	if(CL_TRUEP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		COPY(STACK(base, 0), STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 3));
		Fcdr(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 0));
		if(CL_TRUEP(STACK(base, 1)))
		{
			COPY(STACK(base, 1), STACK(base, 2));
			LOAD_NIL(STACK(base, 3));
			M197_1:;
			if(CL_TRUEP(STACK(base, 0)))
			{
			}
			else
			{
				COPY(STACK(base, 2), STACK(base, 0));
				goto RETURN216;
			}
			COPY(STACK(base, 0), STACK(base, 4));
			Fcar(STACK(base, 4));
			COPY(STACK(base, 0), STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 6));
			Fcdr(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 0));
			COPY(STACK(base, 4), STACK(base, 3));
			if(CL_TRUEP(STACK(base, 3)))
			{
				COPY(STACK(base, 1), STACK(base, 4));
				Flast(STACK(base, 4), 1);
				if(CL_CONSP(STACK(base, 4)))
				{
					COPY(STACK(base, 3), CDR(GET_FORM(STACK(base, 4))));
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KClisp[2], STACK(base, 5));	/* ~a is not a cons */
					COPY(STACK(base, 4), STACK(base, 6));
					Ferror(STACK(base, 5), 2);
				}
				COPY(STACK(base, 3), STACK(base, 1));
			}
			goto M197_1;
			RETURN216:;
		}
		else
		{
			LOAD_GLOBFUN(&CFnconc, STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			Fapply(STACK(base, 2), 2);
			COPY(STACK(base, 2), STACK(base, 0));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
