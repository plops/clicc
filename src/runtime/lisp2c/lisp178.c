#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void ensure_package(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Ffind_package(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 311), STACK(base, 1));	/* USE */
		LOAD_NIL(STACK(base, 2));
		Fmake_package(STACK(base, 0), 3);
	}
}
