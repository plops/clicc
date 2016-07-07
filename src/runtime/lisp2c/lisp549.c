#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFFI_lisp_float)
{
	MAKE_STRING(48, "The evaluated value ~S is not of type c-<float>."),	/* 0 */
};

void FFI_lisp_float(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	c_float_p(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
		bool_result = CL_TRUEP(STACK(base, 1));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		c_double_p(STACK(base, 2));
		if(CL_TRUEP(STACK(base, 2)))
		{
			bool_result = CL_TRUEP(STACK(base, 2));
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 3));
			c_long_double_p(STACK(base, 3));
			bool_result = CL_TRUEP(STACK(base, 3));
		}
	}
	if(bool_result)
	{
		make_lisp_float(STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFFI_lisp_float[0], STACK(base, 1));	/* The evaluated value ~S is not of type c-<float>. */
		COPY(STACK(base, 0), STACK(base, 2));
		Flist(STACK(base, 2), 1);
		LOAD_SMSTR((CL_FORM *)&KClisp[20], STACK(base, 3));	/* ~&Error in ~A: ~?~% */
		LOAD_SYMBOL(SYMBOL(Slisp, 607), STACK(base, 4));	/* LISP-FLOAT */
		COPY(STACK(base, 1), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Ferror(STACK(base, 3), 4);
	}
}
