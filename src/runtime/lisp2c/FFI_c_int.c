#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFFI_c_int)
{
	MAKE_STRING(45, "The evaluated value ~S is not of type fixnum."),	/* 0 */
};

void FFI_c_int(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		make_c_int(STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFFI_c_int[0], STACK(base, 1));	/* The evaluated value ~S is not of type fixnum. */
		COPY(STACK(base, 0), STACK(base, 2));
		Flist(STACK(base, 2), 1);
		LOAD_SMSTR((CL_FORM *)&KClisp[20], STACK(base, 3));	/* ~&Error in ~A: ~?~% */
		LOAD_SYMBOL(SYMBOL(Slisp, 604), STACK(base, 4));	/* C-INT */
		COPY(STACK(base, 1), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Ferror(STACK(base, 3), 4);
	}
}
