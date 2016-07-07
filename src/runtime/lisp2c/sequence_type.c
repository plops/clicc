#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Ksequence_type)
{
	MAKE_STRING(16, "Unexpected error"),	/* 0 */
};

void sequence_type(CL_FORM *base)
{
	if(CL_LISTP(STACK(base, 0)))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 28), STACK(base, 0));	/* LIST */
	}
	else
	{
		if(CL_VECTOR_P(STACK(base, 0)))
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 47), STACK(base, 1));	/* VECTOR */
			COPY(STACK(base, 0), STACK(base, 2));
			Farray_element_type(STACK(base, 2));
			Flist(STACK(base, 1), 2);
			COPY(STACK(base, 1), STACK(base, 0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Ksequence_type[0], STACK(base, 0));	/* Unexpected error */
			Ferror(STACK(base, 0), 1);
		}
	}
}
