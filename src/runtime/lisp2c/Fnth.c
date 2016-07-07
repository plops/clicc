#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fnth(CL_FORM *base)
{
	Fnthcdr(STACK(base, 0));
	Fcar(STACK(base, 0));
}
