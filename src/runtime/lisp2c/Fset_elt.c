#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_elt = {Fset_elt, 3};

void Fset_elt(CL_FORM *base)
{
	if(CL_CONSP(STACK(base, 1)))
	{
		COPY(STACK(base, 2), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 0), STACK(base, 5));
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 2), STACK(base, 7));
		COPY(STACK(base, 1), STACK(base, 8));
		Fnthcdr(STACK(base, 7));
		COPY(STACK(base, 0), STACK(base, 8));
		if(CL_CONSP(STACK(base, 7)))
		{
			COPY(STACK(base, 0), CAR(GET_FORM(STACK(base, 7))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[12], STACK(base, 0));	/* ~a is not a cons */
			COPY(STACK(base, 7), STACK(base, 1));
			Ferror(STACK(base, 0), 2);
		}
	}
	else
	{
		if(CL_VECTOR_P(STACK(base, 1)))
		{
			Fset_row_major_aref(STACK(base, 0));
		}
		else
		{
			COPY(SYMVAL(Slisp, 57), STACK(base, 0));	/* WRONG_TYPE */
			LOAD_SYMBOL(SYMBOL(Slisp, 36), STACK(base, 2));	/* SEQUENCE */
			Ferror(STACK(base, 0), 3);
		}
	}
}
