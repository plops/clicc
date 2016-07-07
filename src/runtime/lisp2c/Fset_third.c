#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_third(CL_FORM *base)
{
	Fset_caddr(STACK(base, 0));
}
