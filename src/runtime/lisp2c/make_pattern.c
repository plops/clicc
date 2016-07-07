#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cmake_pattern = {make_pattern, -1};

void make_pattern(CL_FORM *base, int nargs)
{
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 180),	/* PIECES */
	};
	keysort(STACK(base, 0), nargs - 0, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 0));
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 1));	/* PATTERN */
	COPY(STACK(base, 0), STACK(base, 2));
	make_struct(STACK(base, 1), 2);
	COPY(STACK(base, 1), STACK(base, 0));
}
