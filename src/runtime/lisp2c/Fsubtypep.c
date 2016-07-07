#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFsubtypep)
{
	MAKE_STRING(35, "(SUBTYPEP ~S ~S) is not implemented"),	/* 0 */
};

void Fsubtypep(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Ssys, 1))	/* T */
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFsubtypep[0], STACK(base, 2));	/* (SUBTYPEP ~S ~S) is not implemented */
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
}
