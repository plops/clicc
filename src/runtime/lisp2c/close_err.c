#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cclose_err = {close_err, -1};

CL_INIT2(Kclose_err)
{
	MAKE_STRING(16, "stream is closed"),	/* 0 */
};

void close_err(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	LOAD_SMSTR((CL_FORM *)&Kclose_err[0], STACK(base, 1));	/* stream is closed */
	Ferror(STACK(base, 1), 1);
}
