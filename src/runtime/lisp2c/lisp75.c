#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFno_applicable_method)
{
	MAKE_STRING(58, "No applicable method for generic function ~s with args ~s."),	/* 0 */
};

void Fno_applicable_method(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	LOAD_SMSTR((CL_FORM *)&KFno_applicable_method[0], STACK(base, 2));	/* No applicable method for generic function ~s with args ~s. */
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Ferror(STACK(base, 2), 3);
}
