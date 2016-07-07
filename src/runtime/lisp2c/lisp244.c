#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cmake_readtable = {make_readtable, -1};

void make_readtable(CL_FORM *base, int nargs)
{
	BOOL supl_flags[2];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 388),	/* SYNTAX */
		SYMBOL(Slisp, 389),	/* DISPATCH */
	};
	keysort(STACK(base, 0), nargs - 0, 2, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_FIXNUM(256, STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 3));	/* INITIAL-ELEMENT */
		LOAD_NIL(STACK(base, 4));
		Fmake_array(STACK(base, 2), 3);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 2));	/* READTABLE */
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	make_struct(STACK(base, 2), 3);
	COPY(STACK(base, 2), STACK(base, 0));
}
