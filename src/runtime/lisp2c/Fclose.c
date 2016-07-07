#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fclose(CL_FORM *base, int nargs)
{
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 103),	/* ABORT */
	};
	keysort(STACK(base, 1), nargs - 1, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	COPY(STACK(base, 0), STACK(base, 2));
	stream_close(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}
