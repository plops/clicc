#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcddr(CL_FORM *base)
{
	Fcdr(STACK(base, 0));
	Fcdr(STACK(base, 0));
}
