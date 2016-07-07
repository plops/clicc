#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFstream_element_type)
{
	MAKE_STRING(15, "stream expected"),	/* 0 */
};

void Fstream_element_type(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 2));	/* STREAM */
	struct_typep(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
		LOAD_SMSTR((CL_FORM *)&KFstream_element_type[0], STACK(base, 0));	/* stream expected */
		Ferror(STACK(base, 0), 1);
	}
	else
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 0));	/* CHARACTER */
	}
}
