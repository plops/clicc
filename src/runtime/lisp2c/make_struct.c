#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void make_struct(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	COPY(STACK(base, 1), STACK(base, 2));
	Flength(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	new_struct(STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	M77_1:;
	if(CL_TRUEP(STACK(base, 1)))
	{
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN86;
	}
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	COPY(STACK(base, 1), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 9));
	Fcdr(STACK(base, 9));
	COPY(STACK(base, 9), STACK(base, 1));
	set_struct_ref(STACK(base, 4));
	F1plus(STACK(base, 3));
	goto M77_1;
	RETURN86:;
}
