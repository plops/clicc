#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kcheck_dimensions)
{
	MAKE_STRING(109, "A dimension argument to MAKE-ARRAY, ~A, is unusable ~\n                     since it is not a positive fixnum."),	/* 0 */
};

void check_dimensions(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 1));
	LOAD_FIXNUM(1, STACK(base, 2));
	M6_1:;
	if(CL_TRUEP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN6;
	}
	COPY(STACK(base, 0), STACK(base, 3));
	Fcar(STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 5));
	Fcdr(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 0));
	COPY(STACK(base, 3), STACK(base, 1));
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		LOAD_FIXNUM(0, STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		LOAD_FIXNUM(2147483647, STACK(base, 5));
		Fle(STACK(base, 3), 3);
		bool_result = CL_TRUEP(STACK(base, 3));
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
		LOAD_SMSTR((CL_FORM *)&Kcheck_dimensions[0], STACK(base, 3));	/* A dimension argument to MAKE-ARRAY, ~A, is unusable ~
                     since it is not a positive fixnum. */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	COPY(STACK(base, 1), STACK(base, 3));
	Fmult(STACK(base, 2), 2);
	goto M6_1;
	RETURN6:;
}
