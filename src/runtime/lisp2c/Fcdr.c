#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFcdr)
{
	MAKE_STRING(16, "~a is not a list"),	/* 0 */
};

void Fcdr(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			COPY(GET_CDR(STACK(base, 0)), STACK(base, 0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFcdr[0], STACK(base, 1));	/* ~a is not a list */
			COPY(STACK(base, 0), STACK(base, 2));
			Ferror(STACK(base, 1), 2);
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
