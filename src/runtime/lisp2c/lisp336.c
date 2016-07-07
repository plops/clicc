#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fdelete_duplicates(CL_FORM *base, int nargs)
{
	BOOL supl_flags[6];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 229),	/* FROM-END */
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 1), nargs - 1, 6, keylist, supl_flags, FALSE);
	Flist(STACK(base, 7), nargs - 1);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 6));
	}
	LOAD_GLOBFUN(&CFremove_duplicates, STACK(base, 8));
	COPY(STACK(base, 0), STACK(base, 9));
	COPY(STACK(base, 7), STACK(base, 10));
	Fapply(STACK(base, 8), 3);
	COPY(STACK(base, 8), STACK(base, 0));
}
