#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z107_g1267(CL_FORM *base, int nargs);

void Fcomplement(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	{
		GEN_CLOSURE(array, STACK(base, 1), 4, Z107_g1267, -1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 1));
	}
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z107_g1267(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 0);
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fapply(STACK(base, 2), 2);
	mv_count = 1;
	if(NOT(CL_TRUEP(STACK(base, 2))))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
