#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Faref(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	array_rmi_internal(STACK(base, 3));
	Frow_major_aref(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}
