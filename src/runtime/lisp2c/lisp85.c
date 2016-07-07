#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CPmake_pathname = {Pmake_pathname, -1};

void Pmake_pathname(CL_FORM *base, int nargs)
{
	BOOL supl_flags[6];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 182),	/* HOST */
		SYMBOL(Slisp, 183),	/* DEVICE */
		SYMBOL(Slisp, 184),	/* DIRECTORY */
		SYMBOL(Slisp, 185),	/* NAME */
		SYMBOL(Slisp, 79),	/* TYPE */
		SYMBOL(Slisp, 186),	/* VERSION */
	};
	keysort(STACK(base, 0), nargs - 0, 6, keylist, supl_flags, FALSE);
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
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 2), STACK(base, 9));
	COPY(STACK(base, 3), STACK(base, 10));
	COPY(STACK(base, 4), STACK(base, 11));
	COPY(STACK(base, 5), STACK(base, 12));
	make_struct(STACK(base, 6), 7);
	COPY(STACK(base, 6), STACK(base, 0));
}
