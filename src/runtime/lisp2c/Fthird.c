#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFthird = {Fthird, 1};

void Fthird(CL_FORM *base)
{
	Fcdr(STACK(base, 0));
	Fcdr(STACK(base, 0));
	Fcar(STACK(base, 0));
}
