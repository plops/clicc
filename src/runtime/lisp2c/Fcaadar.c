#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcaadar(CL_FORM *base)
{
	Fcar(STACK(base, 0));
	Fcdr(STACK(base, 0));
	Fcar(STACK(base, 0));
	Fcar(STACK(base, 0));
}
