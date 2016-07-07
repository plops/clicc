#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fmod(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	floor_internal(STACK(base, 2));
	COPY(&mv_buf[0], STACK(base, 3));
	mv_count = 1;
	COPY(STACK(base, 3), STACK(base, 0));
}
