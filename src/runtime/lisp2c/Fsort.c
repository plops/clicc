#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fsort(CL_FORM *base, int nargs)
{
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 2), nargs - 2, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_GLOBFUN(&CFidentity, STACK(base, 2));
	}
	if(CL_LISTP(STACK(base, 0)))
	{
		list_merge_sort(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 3));
		LOAD_FIXNUM(0, STACK(base, 4));
		COPY(STACK(base, 0), STACK(base, 5));
		Flength(STACK(base, 5));
		COPY(STACK(base, 1), STACK(base, 6));
		COPY(STACK(base, 2), STACK(base, 7));
		quick_sort(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 0));
	}
}
