#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fmember_if_not(CL_FORM *base, int nargs)
{
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 2), nargs - 2, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	COPY(STACK(base, 0), STACK(base, 3));
	Fcomplement(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 5));	/* KEY */
	COPY(STACK(base, 2), STACK(base, 6));
	Fmember_if(STACK(base, 3), 4);
	COPY(STACK(base, 3), STACK(base, 0));
}
