#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Felt(CL_FORM *base)
{
	if(CL_CONSP(STACK(base, 0)))
	{
		COPY(STACK(base, 1), STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		Fnthcdr(STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 0));
	}
	else
	{
		if(CL_VECTOR_P(STACK(base, 0)))
		{
			Frow_major_aref(STACK(base, 0));
		}
		else
		{
			COPY(SYMVAL(Slisp, 57), STACK(base, 2));	/* WRONG_TYPE */
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 36), STACK(base, 4));	/* SEQUENCE */
			Ferror(STACK(base, 2), 3);
		}
	}
}
