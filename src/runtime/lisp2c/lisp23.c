#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFget_output_stream_string)
{
	MAKE_STRING(29, "string-output-stream expected"),	/* 0 */
};

void Fget_output_stream_string(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_FIXNUM(0, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 3));	/* STREAM */
	struct_ref(STACK(base, 1));
	if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 98))	/* STRING-OUTPUT */
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFget_output_stream_string[0], STACK(base, 1));	/* string-output-stream expected */
		Ferror(STACK(base, 1), 1);
	}
	stream_extra(STACK(base, 0));
	Ffuncall(STACK(base, 0), 1);
}
