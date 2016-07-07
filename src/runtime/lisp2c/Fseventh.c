#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fseventh(CL_FORM *base)
{
	LOAD_FIXNUM(6, STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	Fnthcdr(STACK(base, 1));
	Fcar(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}
