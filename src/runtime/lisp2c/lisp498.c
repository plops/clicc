#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fstring_not_equal(CL_FORM *base, int nargs)
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
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	LOAD_NIL(STACK(base, 8));
	LOAD_NIL(STACK(base, 9));
	M222_1:;
	COPY(STACK(base, 6), STACK(base, 10));
	COPY(STACK(base, 3), STACK(base, 11));
	Fnumeql(STACK(base, 10), 2);
	if(CL_TRUEP(STACK(base, 10)))
	{
		COPY(STACK(base, 7), STACK(base, 10));
		COPY(STACK(base, 5), STACK(base, 11));
		Fnumeql(STACK(base, 10), 2);
		if(CL_TRUEP(STACK(base, 10)))
		{
			LOAD_NIL(STACK(base, 0));
		}
		else
		{
			COPY(STACK(base, 6), STACK(base, 0));
		}
		goto RETURN259;
	}
	COPY(STACK(base, 7), STACK(base, 10));
	COPY(STACK(base, 5), STACK(base, 11));
	Fnumeql(STACK(base, 10), 2);
	if(CL_TRUEP(STACK(base, 10)))
	{
		COPY(STACK(base, 6), STACK(base, 0));
		goto RETURN259;
	}
	COPY(STACK(base, 0), STACK(base, 10));
	COPY(STACK(base, 6), STACK(base, 11));
	Fchar(STACK(base, 10));
	COPY(STACK(base, 10), STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	COPY(STACK(base, 7), STACK(base, 10));
	Fchar(STACK(base, 9));
	COPY(STACK(base, 8), STACK(base, 10));
	COPY(STACK(base, 9), STACK(base, 11));
	Fchar_equal(STACK(base, 10), 2);
	if(CL_TRUEP(STACK(base, 10)))
	{
	}
	else
	{
		COPY(STACK(base, 6), STACK(base, 0));
		goto RETURN258;
	}
	COPY(STACK(base, 6), STACK(base, 10));
	F1plus(STACK(base, 10));
	COPY(STACK(base, 7), STACK(base, 11));
	F1plus(STACK(base, 11));
	COPY(STACK(base, 11), STACK(base, 7));
	COPY(STACK(base, 10), STACK(base, 6));
	goto M222_1;
	RETURN259:;
	RETURN258:;
}
