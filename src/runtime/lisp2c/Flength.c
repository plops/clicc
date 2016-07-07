#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFlength = {Flength, 1};

CL_INIT2(KFlength)
{
	MAKE_STRING(20, "~a is not a sequence"),	/* 0 */
};

void Flength(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			raw_list_length(STACK(base, 0));
		}
		else
		{
			if(CL_VECTOR_P(STACK(base, 0)))
			{
				LOAD_FIXNUM(AR_SIZE(GET_FORM(STACK(base, 0))), STACK(base, 0));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFlength[0], STACK(base, 1));	/* ~a is not a sequence */
				COPY(STACK(base, 0), STACK(base, 2));
				Ferror(STACK(base, 1), 2);
			}
		}
	}
	else
	{
		LOAD_FIXNUM(0, STACK(base, 0));
	}
}
