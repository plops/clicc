#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFposition = {Fposition, -3};

void Fposition(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 2), nargs - 2);
	if(CL_TRUEP(STACK(base, 1)))
	{
		if(CL_CONSP(STACK(base, 1)))
		{
			LOAD_GLOBFUN(&Clist_position, STACK(base, 3));
			COPY(STACK(base, 0), STACK(base, 4));
			COPY(STACK(base, 1), STACK(base, 5));
			COPY(STACK(base, 2), STACK(base, 6));
			Fapply(STACK(base, 3), 4);
			COPY(STACK(base, 3), STACK(base, 0));
		}
		else
		{
			if(CL_VECTOR_P(STACK(base, 1)))
			{
				LOAD_GLOBFUN(&Cvector_position, STACK(base, 3));
				COPY(STACK(base, 0), STACK(base, 4));
				COPY(STACK(base, 1), STACK(base, 5));
				COPY(STACK(base, 2), STACK(base, 6));
				Fapply(STACK(base, 3), 4);
				COPY(STACK(base, 3), STACK(base, 0));
			}
			else
			{
				COPY(SYMVAL(Slisp, 57), STACK(base, 0));	/* WRONG_TYPE */
				LOAD_SYMBOL(SYMBOL(Slisp, 36), STACK(base, 2));	/* SEQUENCE */
				Ferror(STACK(base, 0), 3);
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
