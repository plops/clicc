#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Frow_major_aref(CL_FORM *base)
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
		Farray_total_size(STACK(base, 4));
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
		COPY(SYMVAL(Slisp, 122), STACK(base, 2));	/* OUT_OF_RANGE */
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Farray_total_size(STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
	row_major_aref_internal(STACK(base, 0));
}
