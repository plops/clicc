#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_fill_pointer = {Fset_fill_pointer, 2};

CL_INIT2(KFset_fill_pointer)
{
	MAKE_STRING(52, "The argument ~A is not a vector with a fill-pointer."),	/* 0 */
	MAKE_STRING(103, "The arg ~S given to SETF of FILL-POINTER is not in ~\n              range for an array of total size ~S."),	/* 2 */
};

void Fset_fill_pointer(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Farray_has_fill_pointer_p(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFset_fill_pointer[0], STACK(base, 2));	/* The argument ~A is not a vector with a fill-pointer. */
		COPY(STACK(base, 1), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		LOAD_FIXNUM(0, STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Farray_total_size(STACK(base, 4));
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
		LOAD_SMSTR((CL_FORM *)&KFset_fill_pointer[2], STACK(base, 2));	/* The arg ~S given to SETF of FILL-POINTER is not in ~
              range for an array of total size ~S. */
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Farray_total_size(STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
	set_fill_pointer_internal(STACK(base, 0));
}
