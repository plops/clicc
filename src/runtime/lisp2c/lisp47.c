#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFarray_dimension)
{
	MAKE_STRING(88, "The value ~A is not suitable as an axis-number ~\n              for an array with rank ~A"),	/* 0 */
};

void Farray_dimension(CL_FORM *base)
{
	if(CL_ARRAY_P(STACK(base, 0)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 121), STACK(base, 2));	/* NO_ARRAY */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		LOAD_FIXNUM(0, STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Farray_rank(STACK(base, 4));
		F1minus(STACK(base, 4));
		Fle(STACK(base, 2), 3);
		bool_result = CL_TRUEP(STACK(base, 2));
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
		LOAD_SMSTR((CL_FORM *)&KFarray_dimension[0], STACK(base, 2));	/* The value ~A is not suitable as an axis-number ~
              for an array with rank ~A */
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Farray_rank(STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
	array_dimension_internal(STACK(base, 0));
}
