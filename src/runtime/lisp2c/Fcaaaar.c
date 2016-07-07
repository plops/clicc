#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcaaaar(CL_FORM *base)
{
	Fcar(STACK(base, 0));
	Fcar(STACK(base, 0));
	Fcar(STACK(base, 0));
	Fcar(STACK(base, 0));
}
