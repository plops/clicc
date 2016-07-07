#include <c_decl.h>
#include "sys.h"
#include "inline.h"

void Fnumberp(CL_FORM *base)
{
	if(CL_NUMBERP(STACK(base, 0)))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
