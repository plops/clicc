#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFstring_left_trim)
{
	MAKE_STRING(0, ""),	/* 0 */
};

void Fstring_left_trim(CL_FORM *base)
{
	Fstring(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	Flength(STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	M225_1:;
	COPY(STACK(base, 3), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fnumeql(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_SMSTR((CL_FORM *)&KFstring_left_trim[0], STACK(base, 0));	/*  */
		goto RETURN263;
	}
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
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
		goto RETURN264;
	}
	F1plus(STACK(base, 3));
	goto M225_1;
	RETURN264:;
	COPY(STACK(base, 3), STACK(base, 4));
	Fzerop(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 0));
		COPY(STACK(base, 3), STACK(base, 1));
		Fsubseq(STACK(base, 0), 2);
	}
	RETURN263:;
}
