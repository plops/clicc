#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFlast)
{
	MAKE_STRING(22, "~A is an illegal index"),	/* 0 */
};

void Flast(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_FIXNUM(1, STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	LOAD_FIXNUM(0, STACK(base, 2));
	if(CL_FIXNUMP(STACK(base, 1)) && GET_FIXNUM(STACK(base, 1)) >= GET_FIXNUM(STACK(base, 2)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFlast[0], STACK(base, 2));	/* ~A is an illegal index */
		COPY(STACK(base, 1), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	Flength(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fminus(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	Fle(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Fnthcdr(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 0));
	}
}
