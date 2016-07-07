#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_second(CL_FORM *base)
{
	Fset_cadr(STACK(base, 0));
}
