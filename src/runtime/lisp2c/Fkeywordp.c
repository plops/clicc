#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fkeywordp(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fsymbol_package(STACK(base, 1));
		if(EQ(STACK(base, 1), SYMVAL(Slisp, 303)))	/* *KEYWORD-PACKAGE* */
		{
			LOAD_T(STACK(base, 0));
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
