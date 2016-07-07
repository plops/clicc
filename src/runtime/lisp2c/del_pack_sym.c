#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void del_pack_sym(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fsvref(STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 310), STACK(base, 5));	/* COUNT */
	LOAD_FIXNUM(1, STACK(base, 6));
	Fdelete(STACK(base, 3), 4);
	COPY(STACK(base, 3), STACK(base, 0));
	Fset_svref(STACK(base, 0));
}
