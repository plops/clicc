#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFsvref)
{
	MAKE_STRING(34, "~A should be of type SIMPLE-VECTOR"),	/* 0 */
};

void Fsvref(CL_FORM *base)
{
	if(CL_SMAR_P(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
		LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
		check_array_internal(STACK(base, 2));
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
		LOAD_SMSTR((CL_FORM *)&KFsvref[0], STACK(base, 2));	/* ~A should be of type SIMPLE-VECTOR */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		LOAD_FIXNUM(0, STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Flength(STACK(base, 4));
		F1minus(STACK(base, 4));
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
		COPY(SYMVAL(Slisp, 122), STACK(base, 2));	/* OUT_OF_RANGE */
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Flength(STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
	svref_internal(STACK(base, 0));
}
