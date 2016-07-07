#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void check_simple_array(CL_FORM *base)
{
	if(CL_SMAR_P(STACK(base, 0)))
	{
		check_array_internal(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
