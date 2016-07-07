#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z108_doit(CL_FORM *base);

void list_nreverse(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	Z108_doit(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z108_doit(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Fcdr(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Fset_cdr(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
		COPY(STACK(base, 2), STACK(base, 1));
		Z108_doit(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 0));
	}
}
