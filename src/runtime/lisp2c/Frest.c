#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Frest(CL_FORM *base)
{
	Fcdr(STACK(base, 0));
}
