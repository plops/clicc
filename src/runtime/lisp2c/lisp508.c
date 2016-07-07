#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fnstring_capitalize(CL_FORM *base, int nargs)
{
	BOOL supl_flags[2];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
	};
	keysort(STACK(base, 1), nargs - 1, 2, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_FIXNUM(0, STACK(base, 1));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Flength(STACK(base, 5));
	check_seq_start_end(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	M232_1:;
	COPY(STACK(base, 3), STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	Fnumeql(STACK(base, 6), 2);
	if(CL_TRUEP(STACK(base, 6)))
	{
		goto RETURN272;
	}
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fschar(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 6));
	Falphanumericp(STACK(base, 6));
	if(CL_TRUEP(STACK(base, 6)))
	{
		if(CL_TRUEP(STACK(base, 4)))
		{
			COPY(STACK(base, 5), STACK(base, 6));
			Fchar_downcase(STACK(base, 6));
		}
		else
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
			COPY(STACK(base, 5), STACK(base, 6));
			Fchar_upcase(STACK(base, 6));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 4));
		COPY(STACK(base, 5), STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 7));
	COPY(STACK(base, 0), STACK(base, 8));
	COPY(STACK(base, 3), STACK(base, 9));
	Fset_schar(STACK(base, 7));
	COPY(STACK(base, 3), STACK(base, 6));
	F1plus(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 3));
	goto M232_1;
	RETURN272:;
}
