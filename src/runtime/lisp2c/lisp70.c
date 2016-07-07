#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFno_next_method)
{
	MAKE_STRING(15, "No next method."),	/* 0 */
};

void Fno_next_method(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&KFno_next_method[0], STACK(base, 0));	/* No next method. */
	Ferror(STACK(base, 0), 1);
}
