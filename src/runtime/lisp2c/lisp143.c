#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kget_unix_error_msg)
{
	MAKE_STRING(18, "Unknown error [~d]"),	/* 0 */
};

void get_unix_error_msg(CL_FORM *base)
{
	COPY(SYMVAL(Slisp, 224), STACK(base, 1));	/* *UNIX-ERRORS* */
	COPY(STACK(base, 0), STACK(base, 2));
	Farray_in_bounds_p(STACK(base, 1), 2);
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(SYMVAL(Slisp, 224), STACK(base, 1));	/* *UNIX-ERRORS* */
		COPY(STACK(base, 0), STACK(base, 2));
		Fsvref(STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 1));
		LOAD_SMSTR((CL_FORM *)&Kget_unix_error_msg[0], STACK(base, 2));	/* Unknown error [~d] */
		COPY(STACK(base, 0), STACK(base, 3));
		Fformat(STACK(base, 1), 3);
		COPY(STACK(base, 1), STACK(base, 0));
	}
}
