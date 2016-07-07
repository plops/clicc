#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Flogior(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	LOAD_FIXNUM(0, STACK(base, 1));
	M43_1:;
	if(CL_TRUEP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 0));
		goto RETURN49;
	}
	COPY(STACK(base, 0), STACK(base, 2));
	Fcar(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 4));
	Fcdr(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 0));
	if(CL_FIXNUMP(STACK(base, 2)))
	{
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 3));
		LOAD_SMSTR((CL_FORM *)&KClisp[8], STACK(base, 2));	/* type error: ~S is not of type ~S */
		LOAD_SYMBOL(SYMBOL(Slisp, 23), STACK(base, 4));	/* FIXNUM */
		Ferror(STACK(base, 2), 3);
	}
	LOAD_FIXNUM(GET_FIXNUM(STACK(base, 1)) | GET_FIXNUM(STACK(base, 2)), STACK(base, 1));
	goto M43_1;
	RETURN49:;
}
