#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_fourth(CL_FORM *base)
{
	Fset_cadddr(STACK(base, 0));
}
