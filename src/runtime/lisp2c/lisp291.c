#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fprin1_to_string(CL_FORM *base)
{
	Fmake_string_output_stream(STACK(base, 1), 0);
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fprin1(STACK(base, 2), 2);
	COPY(STACK(base, 1), STACK(base, 0));
	Fget_output_stream_string(STACK(base, 0));
}
