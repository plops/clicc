#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFcharacter)
{
	MAKE_STRING(34, "cannot coerce ~S to type character"),	/* 0 */
};

void Fcharacter(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
	{
		Fsymbol_name(STACK(base, 0));
	}
	if(CL_STRING_P(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Flength(STACK(base, 1));
		bool_result = CL_FIXNUMP(STACK(base, 1)) && GET_FIXNUM(STACK(base, 1)) == 1;
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_FIXNUM(0, STACK(base, 1));
		Fchar(STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFcharacter[0], STACK(base, 1));	/* cannot coerce ~S to type character */
		COPY(STACK(base, 0), STACK(base, 2));
		Ferror(STACK(base, 1), 2);
	}
}
