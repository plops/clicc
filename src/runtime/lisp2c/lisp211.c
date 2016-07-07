#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void set_struct_ref(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	struct_typep(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 326), STACK(base, 4));	/* NO_STRUCT */
		COPY(STACK(base, 0), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Ferror(STACK(base, 4), 3);
	}
	COPY(STACK(base, 3), STACK(base, 2));
	set_struct_ref_internal(STACK(base, 0));
}
