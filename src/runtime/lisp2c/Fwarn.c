#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFwarn)
{
	MAKE_STRING(15, "~&Warning: ~?~%"),	/* 0 */
};

void Fwarn(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	COPY(SYMVAL(Slisp, 435), STACK(base, 2));	/* *ERROR-PRINT-LENGTH* */
	COPY(SYMVAL(Slisp, 436), STACK(base, 3));	/* *ERROR-PRINT-LEVEL* */
	COPY(STACK(base, 2), STACK(base, 4));
	BIND_SPECIAL(SYMBOL(Slisp, 405), STACK(base, 4));	/* *PRINT-LENGTH* */
	COPY(STACK(base, 3), STACK(base, 5));
	BIND_SPECIAL(SYMBOL(Slisp, 406), STACK(base, 5));	/* *PRINT-LEVEL* */
	COPY(SYMVAL(Slisp, 61), STACK(base, 6));	/* *ERROR-OUTPUT* */
	LOAD_SMSTR((CL_FORM *)&KFwarn[0], STACK(base, 7));	/* ~&Warning: ~?~% */
	COPY(STACK(base, 0), STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	Fformat(STACK(base, 6), 4);
	COPY(STACK(base, 6), STACK(base, 0));
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
}
