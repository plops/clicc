#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fnreconc(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	Freverse(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fnconc(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}
