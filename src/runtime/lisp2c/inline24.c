#include <c_decl.h>
#include "sys.h"
#include "inline.h"

void c_unsigned_char_p(CL_FORM *base)
{
	if(CL_C_UNSIGNED_CHAR_P(STACK(base, 0)))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
