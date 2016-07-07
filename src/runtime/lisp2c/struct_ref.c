#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void struct_ref(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	struct_typep(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 326), STACK(base, 3));	/* NO_STRUCT */
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	struct_ref_internal(STACK(base, 0));
}
