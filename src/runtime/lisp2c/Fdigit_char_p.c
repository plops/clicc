#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFdigit_char_p)
{
	MAKE_STRING(27, "~A should be suitable radix"),	/* 0 */
};

void Fdigit_char_p(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_FIXNUM(10, STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		LOAD_FIXNUM(2, STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		LOAD_FIXNUM(36, STACK(base, 4));
		Fle(STACK(base, 2), 3);
		bool_result = CL_TRUEP(STACK(base, 2));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFdigit_char_p[0], STACK(base, 2));	/* ~A should be suitable radix */
		COPY(STACK(base, 1), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	digit_char_p_internal(STACK(base, 0));
}
