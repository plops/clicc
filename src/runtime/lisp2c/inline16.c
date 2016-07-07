#include <c_decl.h>
#include "sys.h"
#include "inline.h"

void Fsimple_vector_p(CL_FORM *base)
{
	if(CL_SMVEC_T_P(STACK(base, 0)))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
