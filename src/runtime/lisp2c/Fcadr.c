#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFcadr = {Fcadr, 1};

void Fcadr(CL_FORM *base)
{
	Fcdr(STACK(base, 0));
	Fcar(STACK(base, 0));
}
