#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_row_major_aref = {Fset_row_major_aref, 3};

CL_INIT2(KFset_row_major_aref)
{
	MAKE_STRING(60, "The value ~A, is the wrong type for storing into a ~A array."),	/* 0 */
};

void Fset_row_major_aref(CL_FORM *base)
{
	if(CL_ARRAY_P(STACK(base, 1)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 121), STACK(base, 3));	/* NO_ARRAY */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	if(CL_FIXNUMP(STACK(base, 2)))
	{
		LOAD_FIXNUM(0, STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Farray_total_size(STACK(base, 5));
		F1minus(STACK(base, 5));
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
		COPY(SYMVAL(Slisp, 122), STACK(base, 3));	/* OUT_OF_RANGE */
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Farray_total_size(STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	array_element_type_internal(STACK(base, 4));
	type_code_p(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFset_row_major_aref[0], STACK(base, 3));	/* The value ~A, is the wrong type for storing into a ~A array. */
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Farray_element_type(STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	set_row_major_aref_internal(STACK(base, 0));
}
