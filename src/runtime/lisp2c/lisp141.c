#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fdocumentation(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 0));
}
