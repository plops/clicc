#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFarray_in_bounds_p = {Farray_in_bounds_p, -2};

CL_INIT2(KFarray_in_bounds_p)
{
	MAKE_STRING(39, "Wrong number of subscripts for array ~a"),	/* 0 */
};

void Farray_in_bounds_p(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	COPY(STACK(base, 0), STACK(base, 2));
	Farray_rank(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Flength(STACK(base, 3));
	Fnumneql(STACK(base, 2), 2);
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KFarray_in_bounds_p[0], STACK(base, 2));	/* Wrong number of subscripts for array ~a */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	LOAD_FIXNUM(0, STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	M8_1:;
	if(CL_ATOMP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 3));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
		goto RETURN8;
	}
	COPY(STACK(base, 4), STACK(base, 5));
	Fcar(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 3));
	if(CL_FIXNUMP(STACK(base, 3)))
	{
		LOAD_FIXNUM(0, STACK(base, 5));
		COPY(STACK(base, 3), STACK(base, 6));
		COPY(STACK(base, 0), STACK(base, 7));
		COPY(STACK(base, 2), STACK(base, 8));
		Farray_dimension(STACK(base, 7));
		F1minus(STACK(base, 7));
		Fle(STACK(base, 5), 3);
		bool_result = CL_TRUEP(STACK(base, 5));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN8;
	}
	COPY(STACK(base, 2), STACK(base, 5));
	F1plus(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 2));
	Fcdr(STACK(base, 4));
	goto M8_1;
	RETURN8:;
}
