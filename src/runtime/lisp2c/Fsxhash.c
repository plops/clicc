#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fsxhash(CL_FORM *base)
{
	LOAD_FIXNUM(0, STACK(base, 1));
	internal_sxhash(STACK(base, 0));
}
