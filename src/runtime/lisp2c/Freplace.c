#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Freplace(CL_FORM *base, int nargs)
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
	Fmin(STACK(base, 6), 2);
	if(EQ(STACK(base, 0), STACK(base, 1)))
	{
		COPY(STACK(base, 2), STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 8));
		Fgt(STACK(base, 7), 2);
		bool_result = CL_TRUEP(STACK(base, 7));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_FIXNUM(0, STACK(base, 7));
		COPY(STACK(base, 2), STACK(base, 8));
		COPY(STACK(base, 6), STACK(base, 9));
		F1minus(STACK(base, 9));
		Fplus(STACK(base, 8), 2);
		COPY(STACK(base, 4), STACK(base, 9));
		COPY(STACK(base, 6), STACK(base, 10));
		F1minus(STACK(base, 10));
		Fplus(STACK(base, 9), 2);
		M159_1:;
		COPY(STACK(base, 7), STACK(base, 10));
		COPY(STACK(base, 6), STACK(base, 11));
		Fge(STACK(base, 10), 2);
		if(CL_TRUEP(STACK(base, 10)))
		{
			goto RETURN173;
		}
		COPY(STACK(base, 1), STACK(base, 10));
		COPY(STACK(base, 9), STACK(base, 11));
		Felt(STACK(base, 10));
		COPY(STACK(base, 10), STACK(base, 11));
		COPY(STACK(base, 0), STACK(base, 12));
		COPY(STACK(base, 8), STACK(base, 13));
		Fset_elt(STACK(base, 11));
		COPY(STACK(base, 7), STACK(base, 10));
		F1plus(STACK(base, 10));
		COPY(STACK(base, 8), STACK(base, 11));
		F1minus(STACK(base, 11));
		COPY(STACK(base, 9), STACK(base, 12));
		F1minus(STACK(base, 12));
		COPY(STACK(base, 12), STACK(base, 9));
		COPY(STACK(base, 11), STACK(base, 8));
		COPY(STACK(base, 10), STACK(base, 7));
		goto M159_1;
		RETURN173:;
	}
	else
	{
		LOAD_FIXNUM(0, STACK(base, 7));
		COPY(STACK(base, 2), STACK(base, 8));
		COPY(STACK(base, 4), STACK(base, 9));
		M160_1:;
		COPY(STACK(base, 7), STACK(base, 10));
		COPY(STACK(base, 6), STACK(base, 11));
		Fge(STACK(base, 10), 2);
		if(CL_TRUEP(STACK(base, 10)))
		{
			goto RETURN174;
		}
		COPY(STACK(base, 1), STACK(base, 10));
		COPY(STACK(base, 9), STACK(base, 11));
		Felt(STACK(base, 10));
		COPY(STACK(base, 10), STACK(base, 11));
		COPY(STACK(base, 0), STACK(base, 12));
		COPY(STACK(base, 8), STACK(base, 13));
		Fset_elt(STACK(base, 11));
		COPY(STACK(base, 7), STACK(base, 10));
		F1plus(STACK(base, 10));
		COPY(STACK(base, 8), STACK(base, 11));
		F1plus(STACK(base, 11));
		COPY(STACK(base, 9), STACK(base, 12));
		F1plus(STACK(base, 12));
		COPY(STACK(base, 12), STACK(base, 9));
		COPY(STACK(base, 11), STACK(base, 8));
		COPY(STACK(base, 10), STACK(base, 7));
		goto M160_1;
		RETURN174:;
	}
}
