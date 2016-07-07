#include <c_decl.h>
#include "sys.h"
#include "inline.h"

void Fnot(CL_FORM *base)
{
	if(NOT(CL_TRUEP(STACK(base, 0))))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
