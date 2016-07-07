#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFstring_trim)
{
	MAKE_STRING(0, ""),	/* 0 */
};

void Fstring_trim(CL_FORM *base)
{
	Fstring(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	Flength(STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	M223_1:;
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	Fnumeql(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		LOAD_SMSTR((CL_FORM *)&KFstring_trim[0], STACK(base, 0));	/*  */
		goto RETURN260;
	}
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fchar(STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 7));	/* TEST */
	{
		static GLOBAL_FUNARG extern_closure = {FcharE, -2};
		LOAD_GLOBFUN(&extern_closure, STACK(base, 8));
	}
	Ffind(STACK(base, 5), 4);
	if(CL_TRUEP(STACK(base, 5)))
	{
	}
	else
	{
		goto RETURN261;
	}
	COPY(STACK(base, 3), STACK(base, 5));
	F1plus(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 3));
	goto M223_1;
	RETURN261:;
	M224_1:;
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 4), STACK(base, 6));
	F1minus(STACK(base, 6));
	Fnumeql(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		goto RETURN262;
	}
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 4), STACK(base, 6));
	F1minus(STACK(base, 6));
	Fchar(STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 7));	/* TEST */
	{
		static GLOBAL_FUNARG extern_closure = {FcharE, -2};
		LOAD_GLOBFUN(&extern_closure, STACK(base, 8));
	}
	Ffind(STACK(base, 5), 4);
	if(CL_TRUEP(STACK(base, 5)))
	{
	}
	else
	{
		goto RETURN262;
	}
	F1minus(STACK(base, 4));
	goto M224_1;
	RETURN262:;
	COPY(STACK(base, 3), STACK(base, 5));
	Fzerop(STACK(base, 5));
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 4), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Fnumeql(STACK(base, 5), 2);
		bool_result = CL_TRUEP(STACK(base, 5));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 0));
		COPY(STACK(base, 3), STACK(base, 1));
		COPY(STACK(base, 4), STACK(base, 2));
		Fsubseq(STACK(base, 0), 3);
	}
	RETURN260:;
}
