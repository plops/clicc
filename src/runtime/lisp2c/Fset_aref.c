#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_aref = {Fset_aref, -3};

void Fset_aref(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 2), nargs - 2);
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	array_rmi_internal(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 2));
	Fset_row_major_aref(STACK(base, 0));
}
