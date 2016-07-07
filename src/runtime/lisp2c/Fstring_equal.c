#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fstring_equal(CL_FORM *base, int nargs)
{
	BOOL supl_flags[4];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 237),	/* START1 */
		SYMBOL(Slisp, 238),	/* END1 */
		SYMBOL(Slisp, 211),	/* START2 */
		SYMBOL(Slisp, 212),	/* END2 */
	};
	keysort(STACK(base, 2), nargs - 2, 4, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_FIXNUM(0, STACK(base, 2));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_FIXNUM(0, STACK(base, 4));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	COPY(STACK(base, 0), STACK(base, 6));
	Fstring(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 0));
	COPY(STACK(base, 1), STACK(base, 6));
	Fstring(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 1));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 3), STACK(base, 7));
	COPY(STACK(base, 0), STACK(base, 8));
	Flength(STACK(base, 8));
	check_seq_start_end(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 3));
	COPY(STACK(base, 4), STACK(base, 6));
	COPY(STACK(base, 5), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	Flength(STACK(base, 8));
	check_seq_start_end(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	Fminus(STACK(base, 6), 2);
	COPY(STACK(base, 5), STACK(base, 7));
	COPY(STACK(base, 4), STACK(base, 8));
	Fminus(STACK(base, 7), 2);
	Fnumeql(STACK(base, 6), 2);
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 2), STACK(base, 6));
		COPY(STACK(base, 4), STACK(base, 7));
		M212_1:;
		COPY(STACK(base, 6), STACK(base, 8));
		COPY(STACK(base, 3), STACK(base, 9));
		Fnumeql(STACK(base, 8), 2);
		if(CL_TRUEP(STACK(base, 8)))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
			goto RETURN239;
		}
		COPY(STACK(base, 0), STACK(base, 8));
		COPY(STACK(base, 6), STACK(base, 9));
		Fchar(STACK(base, 8));
		COPY(STACK(base, 1), STACK(base, 9));
		COPY(STACK(base, 7), STACK(base, 10));
		Fchar(STACK(base, 9));
		Fchar_equal(STACK(base, 8), 2);
		if(CL_TRUEP(STACK(base, 8)))
		{
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN238;
		}
		COPY(STACK(base, 6), STACK(base, 8));
		F1plus(STACK(base, 8));
		COPY(STACK(base, 7), STACK(base, 9));
		F1plus(STACK(base, 9));
		COPY(STACK(base, 9), STACK(base, 7));
		COPY(STACK(base, 8), STACK(base, 6));
		goto M212_1;
		RETURN239:;
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
	RETURN238:;
}
