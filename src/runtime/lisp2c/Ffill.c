#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Ffill(CL_FORM *base, int nargs)
{
	BOOL supl_flags[2];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
	};
	keysort(STACK(base, 2), nargs - 2, 2, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_FIXNUM(0, STACK(base, 2));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	Flength(STACK(base, 6));
	check_seq_start_end(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	M158_1:;
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		goto RETURN172;
	}
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fset_elt(STACK(base, 5));
	F1plus(STACK(base, 4));
	goto M158_1;
	RETURN172:;
}
