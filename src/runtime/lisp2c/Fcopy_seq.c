#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fcopy_seq(CL_FORM *base)
{
	LOAD_FIXNUM(0, STACK(base, 1));
	Fsubseq(STACK(base, 0), 2);
}
