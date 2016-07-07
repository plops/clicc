#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void quick_sort(CL_FORM *base)
{
	LOAD_FIXNUM(0, STACK(base, 5));
	LOAD_FIXNUM(0, STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	F1plus(STACK(base, 8));
	Fle(STACK(base, 7), 2);
	if(CL_TRUEP(STACK(base, 7)))
	{
		goto RETURN209;
	}
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	F1minus(STACK(base, 6));
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	Felt(STACK(base, 7));
	M191_1:;
	COPY(STACK(base, 5), STACK(base, 8));
	COPY(STACK(base, 6), STACK(base, 9));
	Fgt(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		goto RETURN210;
	}
	M192_1:;
	COPY(STACK(base, 5), STACK(base, 8));
	COPY(STACK(base, 6), STACK(base, 9));
	Fgt(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		bool_result = TRUE;
	}
	else
	{
		COPY(STACK(base, 3), STACK(base, 9));
		COPY(STACK(base, 4), STACK(base, 10));
		COPY(STACK(base, 0), STACK(base, 11));
		COPY(STACK(base, 6), STACK(base, 12));
		Felt(STACK(base, 11));
		Ffuncall(STACK(base, 10), 2);
		mv_count = 1;
		COPY(STACK(base, 4), STACK(base, 11));
		COPY(STACK(base, 7), STACK(base, 12));
		Ffuncall(STACK(base, 11), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 9), 3);
		mv_count = 1;
		bool_result = CL_TRUEP(STACK(base, 9));
	}
	if(bool_result)
	{
		goto RETURN211;
	}
	COPY(STACK(base, 6), STACK(base, 8));
	F1minus(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 6));
	goto M192_1;
	RETURN211:;
	COPY(STACK(base, 6), STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	Flt(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		COPY(STACK(base, 0), STACK(base, 8));
		COPY(STACK(base, 1), STACK(base, 9));
		F1plus(STACK(base, 9));
		COPY(STACK(base, 2), STACK(base, 10));
		COPY(STACK(base, 3), STACK(base, 11));
		COPY(STACK(base, 4), STACK(base, 12));
		quick_sort(STACK(base, 8));
		goto RETURN209;
	}
	M193_1:;
	COPY(STACK(base, 5), STACK(base, 8));
	COPY(STACK(base, 6), STACK(base, 9));
	Fgt(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		bool_result = TRUE;
	}
	else
	{
		COPY(STACK(base, 3), STACK(base, 9));
		COPY(STACK(base, 4), STACK(base, 10));
		COPY(STACK(base, 0), STACK(base, 11));
		COPY(STACK(base, 5), STACK(base, 12));
		Felt(STACK(base, 11));
		Ffuncall(STACK(base, 10), 2);
		mv_count = 1;
		COPY(STACK(base, 4), STACK(base, 11));
		COPY(STACK(base, 7), STACK(base, 12));
		Ffuncall(STACK(base, 11), 2);
		mv_count = 1;
		Ffuncall(STACK(base, 9), 3);
		mv_count = 1;
		bool_result = NOT(CL_TRUEP(STACK(base, 9)));
	}
	if(bool_result)
	{
		goto RETURN212;
	}
	COPY(STACK(base, 5), STACK(base, 8));
	F1plus(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 5));
	goto M193_1;
	RETURN212:;
	COPY(STACK(base, 5), STACK(base, 8));
	COPY(STACK(base, 6), STACK(base, 9));
	Fgt(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		goto RETURN210;
	}
	COPY(STACK(base, 0), STACK(base, 8));
	COPY(STACK(base, 5), STACK(base, 9));
	Felt(STACK(base, 8));
	COPY(STACK(base, 0), STACK(base, 9));
	COPY(STACK(base, 6), STACK(base, 10));
	Felt(STACK(base, 9));
	COPY(STACK(base, 9), STACK(base, 10));
	COPY(STACK(base, 0), STACK(base, 11));
	COPY(STACK(base, 5), STACK(base, 12));
	Fset_elt(STACK(base, 10));
	COPY(STACK(base, 8), STACK(base, 10));
	COPY(STACK(base, 0), STACK(base, 11));
	COPY(STACK(base, 6), STACK(base, 12));
	Fset_elt(STACK(base, 10));
	COPY(STACK(base, 5), STACK(base, 8));
	F1plus(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 5));
	COPY(STACK(base, 6), STACK(base, 8));
	F1minus(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 6));
	goto M191_1;
	RETURN210:;
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 5), STACK(base, 9));
	COPY(STACK(base, 3), STACK(base, 10));
	COPY(STACK(base, 4), STACK(base, 11));
	quick_sort(STACK(base, 7));
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	COPY(STACK(base, 2), STACK(base, 9));
	COPY(STACK(base, 3), STACK(base, 10));
	COPY(STACK(base, 4), STACK(base, 11));
	quick_sort(STACK(base, 7));
	RETURN209:;
}
