#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFstring_right_trim)
{
	MAKE_STRING(0, ""),	/* 0 */
};

void Fstring_right_trim(CL_FORM *base)
{
	Fstring(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	Flength(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 3));
	M226_1:;
	COPY(STACK(base, 3), STACK(base, 4));
	Fzerop(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_SMSTR((CL_FORM *)&KFstring_right_trim[0], STACK(base, 0));	/*  */
		goto RETURN265;
	}
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	F1minus(STACK(base, 5));
	Fchar(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 6));	/* TEST */
	{
		static GLOBAL_FUNARG extern_closure = {FcharE, -2};
		LOAD_GLOBFUN(&extern_closure, STACK(base, 7));
	}
	Ffind(STACK(base, 4), 4);
	if(CL_TRUEP(STACK(base, 4)))
	{
	}
	else
	{
		goto RETURN266;
	}
	F1minus(STACK(base, 3));
	goto M226_1;
	RETURN266:;
	COPY(STACK(base, 3), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fnumeql(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 0));
		LOAD_FIXNUM(0, STACK(base, 1));
		COPY(STACK(base, 3), STACK(base, 2));
		Fsubseq(STACK(base, 0), 3);
	}
	RETURN265:;
}
