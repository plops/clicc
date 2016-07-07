#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fsecond(CL_FORM *base)
{
	Fcdr(STACK(base, 0));
	Fcar(STACK(base, 0));
}
