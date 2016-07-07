#include <c_decl.h>
#include "sys.h"
#include "inline.h"

void Feq(CL_FORM *base)
{
	if(EQ(STACK(base, 0), STACK(base, 1)))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
