#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cundef_stream_op = {undef_stream_op, -1};

CL_INIT2(Kundef_stream_op)
{
	MAKE_STRING(26, "undefined Stream Operation"),	/* 0 */
};

void undef_stream_op(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	LOAD_SMSTR((CL_FORM *)&Kundef_stream_op[0], STACK(base, 1));	/* undefined Stream Operation */
	Ferror(STACK(base, 1), 1);
}
