#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flist_all_packages(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 28), STACK(base, 0));	/* LIST */
	COPY(SYMVAL(Slisp, 302), STACK(base, 1));	/* *PACKAGE-ARRAY* */
	Fconcatenate(STACK(base, 0), 2);
}
