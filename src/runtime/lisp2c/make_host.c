#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cmake_host = {make_host, -1};

void make_host(CL_FORM *base, int nargs)
{
	BOOL supl_flags[7];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 187),	/* PARSE */
		SYMBOL(Slisp, 188),	/* UNPARSE */
		SYMBOL(Slisp, 189),	/* UNPARSE-HOST */
		SYMBOL(Slisp, 190),	/* UNPARSE-DIRECTORY */
		SYMBOL(Slisp, 191),	/* UNPARSE-FILE */
		SYMBOL(Slisp, 192),	/* UNPARSE-ENOUGH */
		SYMBOL(Slisp, 193),	/* CUSTOMARY-CASE */
	};
	keysort(STACK(base, 0), nargs - 0, 7, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 0));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_NIL(STACK(base, 6));
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 7));	/* HOST */
	COPY(STACK(base, 0), STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	COPY(STACK(base, 2), STACK(base, 10));
	COPY(STACK(base, 3), STACK(base, 11));
	COPY(STACK(base, 4), STACK(base, 12));
	COPY(STACK(base, 5), STACK(base, 13));
	COPY(STACK(base, 6), STACK(base, 14));
	make_struct(STACK(base, 7), 8);
	COPY(STACK(base, 7), STACK(base, 0));
}
