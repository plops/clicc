#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fset_sixth(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(5, STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Fset_nth(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}
