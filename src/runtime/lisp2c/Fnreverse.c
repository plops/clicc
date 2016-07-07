#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fnreverse(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			list_nreverse(STACK(base, 0));
		}
		else
		{
			if(CL_VECTOR_P(STACK(base, 0)))
			{
				COPY(STACK(base, 0), STACK(base, 1));
				LOAD_FIXNUM(0, STACK(base, 2));
				COPY(STACK(base, 0), STACK(base, 3));
				Flength(STACK(base, 3));
				F1minus(STACK(base, 3));
				LOAD_NIL(STACK(base, 4));
				M140_1:;
				COPY(STACK(base, 2), STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				Fge(STACK(base, 5), 2);
				if(CL_TRUEP(STACK(base, 5)))
				{
					goto RETURN154;
				}
				COPY(STACK(base, 0), STACK(base, 4));
				COPY(STACK(base, 2), STACK(base, 5));
				Felt(STACK(base, 4));
				COPY(STACK(base, 0), STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				Felt(STACK(base, 5));
				COPY(STACK(base, 5), STACK(base, 6));
				COPY(STACK(base, 0), STACK(base, 7));
				COPY(STACK(base, 2), STACK(base, 8));
				Fset_elt(STACK(base, 6));
				COPY(STACK(base, 4), STACK(base, 5));
				COPY(STACK(base, 0), STACK(base, 6));
				COPY(STACK(base, 3), STACK(base, 7));
				Fset_elt(STACK(base, 5));
				COPY(STACK(base, 2), STACK(base, 5));
				F1plus(STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				F1minus(STACK(base, 6));
				COPY(STACK(base, 6), STACK(base, 3));
				COPY(STACK(base, 5), STACK(base, 2));
				goto M140_1;
				RETURN154:;
			}
			else
			{
				COPY(SYMVAL(Slisp, 57), STACK(base, 1));	/* WRONG_TYPE */
				COPY(STACK(base, 0), STACK(base, 2));
				LOAD_SYMBOL(SYMBOL(Slisp, 36), STACK(base, 3));	/* SEQUENCE */
				Ferror(STACK(base, 1), 3);
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
