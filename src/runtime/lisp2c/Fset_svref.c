#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_svref = {Fset_svref, 3};

void Fset_svref(CL_FORM *base)
{
	if(CL_SMVEC_T_P(STACK(base, 1)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 57), STACK(base, 3));	/* WRONG_TYPE */
		COPY(STACK(base, 1), STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Slisp, 41), STACK(base, 5));	/* SIMPLE-VECTOR */
		Ferror(STACK(base, 3), 3);
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
	set_svref_internal(STACK(base, 0));
}
