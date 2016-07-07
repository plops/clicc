#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Farray_row_major_index(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	array_rmi_internal(STACK(base, 0));
}
