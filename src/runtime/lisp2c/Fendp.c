#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFendp)
{
	MAKE_STRING(16, "~a is not a list"),	/* 0 */
};

void Fendp(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			LOAD_NIL(STACK(base, 0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFendp[0], STACK(base, 1));	/* ~a is not a list */
			COPY(STACK(base, 0), STACK(base, 2));
			Ferror(STACK(base, 1), 2);
		}
	}
	else
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
}
