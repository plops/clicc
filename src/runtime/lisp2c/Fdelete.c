#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fdelete(CL_FORM *base, int nargs)
{
	BOOL supl_flags[7];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 229),	/* FROM-END */
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
		SYMBOL(Slisp, 310),	/* COUNT */
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 2), nargs - 2, 7, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_FIXNUM(0, STACK(base, 5));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 6));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 7));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_GLOBFUN(&CFidentity, STACK(base, 8));
	}
	COPY(STACK(base, 0), STACK(base, 9));
	COPY(STACK(base, 1), STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Slisp, 229), STACK(base, 11));	/* FROM-END */
	COPY(STACK(base, 2), STACK(base, 12));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 13));	/* TEST */
	COPY(STACK(base, 3), STACK(base, 14));
	LOAD_SYMBOL(SYMBOL(Slisp, 479), STACK(base, 15));	/* TEST-NOT */
	COPY(STACK(base, 4), STACK(base, 16));
	LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 17));	/* START */
	COPY(STACK(base, 5), STACK(base, 18));
	LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 19));	/* END */
	COPY(STACK(base, 6), STACK(base, 20));
	LOAD_SYMBOL(SYMBOL(Slisp, 310), STACK(base, 21));	/* COUNT */
	COPY(STACK(base, 7), STACK(base, 22));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 23));	/* KEY */
	COPY(STACK(base, 8), STACK(base, 24));
	Fremove(STACK(base, 9), 16);
	COPY(STACK(base, 9), STACK(base, 0));
}
