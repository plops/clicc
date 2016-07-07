#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void copy_struct(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	struct_typep(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 326), STACK(base, 2));	/* NO_STRUCT */
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	struct_size(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	new_struct(STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	M78_1:;
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		goto RETURN87;
	}
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 4), STACK(base, 6));
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 4), STACK(base, 8));
	struct_ref_internal(STACK(base, 7));
	set_struct_ref_internal(STACK(base, 5));
	F1plus(STACK(base, 4));
	goto M78_1;
	RETURN87:;
	COPY(STACK(base, 3), STACK(base, 0));
}
