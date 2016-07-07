#include <c_decl.h>
#include "sys.h"
#include "inline.h"

void Feql(CL_FORM *base)
{
	if(EQ(STACK(base, 0), STACK(base, 1)) || CL_FLOATP(STACK(base, 0)) && GET_FLOAT(STACK(base, 0)) == GET_FLOAT(STACK(base, 1)))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
