#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Ffile_length(CL_FORM *base)
{
	stream_length(STACK(base, 0));
	Ffuncall(STACK(base, 0), 1);
}
