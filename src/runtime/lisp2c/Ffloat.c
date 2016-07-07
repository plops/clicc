#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFfloat)
{
	MAKE_STRING(41, "The value of OTHER, ~S, should be a FLOAT"),	/* 0 */
};

void Ffloat(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		bool_result = NOT(CL_FLOATP(STACK(base, 1)));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_SMSTR((CL_FORM *)&KFfloat[0], STACK(base, 2));	/* The value of OTHER, ~S, should be a FLOAT */
		COPY(STACK(base, 1), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	float_internal(STACK(base, 0));
}
