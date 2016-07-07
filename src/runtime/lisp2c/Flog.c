#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flog(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		{
			static double local_float = 2.7182818284590455;
			LOAD_FLOAT(&local_float, STACK(base, 1));
		}
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	log_internal(STACK(base, 0));
}
