#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Farray_element_type(CL_FORM *base)
{
	if(CL_ARRAY_P(STACK(base, 0)))
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 121), STACK(base, 1));	/* NO_ARRAY */
		COPY(STACK(base, 0), STACK(base, 2));
		Ferror(STACK(base, 1), 2);
	}
	array_element_type_internal(STACK(base, 0));
	to_element_type(STACK(base, 0));
}
