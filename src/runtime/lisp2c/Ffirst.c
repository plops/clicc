#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Ffirst(CL_FORM *base)
{
	Fcar(STACK(base, 0));
}
