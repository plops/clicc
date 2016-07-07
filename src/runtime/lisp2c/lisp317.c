#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cconcatenate_to_non_list = {concatenate_to_non_list, -2};

void concatenate_to_non_list(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	LOAD_NIL(STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	M144_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN158;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Flength(STACK(base, 8));
	Fplus(STACK(base, 7), 2);
	COPY(STACK(base, 7), STACK(base, 3));
	Fcdr(STACK(base, 6));
	goto M144_1;
	RETURN158:;
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fmake_sequence(STACK(base, 5), 2);
	COPY(STACK(base, 5), STACK(base, 2));
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	M145_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN159;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	if(CL_LISTP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 7));
		COPY(STACK(base, 5), STACK(base, 8));
		M146_1:;
		if(CL_ATOMP(STACK(base, 8)))
		{
			LOAD_NIL(STACK(base, 7));
			goto RETURN160;
		}
		COPY(STACK(base, 8), STACK(base, 9));
		Fcar(STACK(base, 9));
		COPY(STACK(base, 9), STACK(base, 7));
		COPY(STACK(base, 2), STACK(base, 10));
		COPY(STACK(base, 4), STACK(base, 11));
		Fset_elt(STACK(base, 9));
		COPY(STACK(base, 4), STACK(base, 9));
		F1plus(STACK(base, 9));
		COPY(STACK(base, 9), STACK(base, 4));
		Fcdr(STACK(base, 8));
		goto M146_1;
		RETURN160:;
	}
	else
	{
		COPY(STACK(base, 5), STACK(base, 7));
		Flength(STACK(base, 7));
		LOAD_FIXNUM(0, STACK(base, 8));
		M147_1:;
		COPY(STACK(base, 8), STACK(base, 9));
		COPY(STACK(base, 7), STACK(base, 10));
		Fge(STACK(base, 9), 2);
		if(CL_TRUEP(STACK(base, 9)))
		{
			goto RETURN161;
		}
		COPY(STACK(base, 5), STACK(base, 9));
		COPY(STACK(base, 8), STACK(base, 10));
		Felt(STACK(base, 9));
		COPY(STACK(base, 9), STACK(base, 10));
		COPY(STACK(base, 2), STACK(base, 11));
		COPY(STACK(base, 4), STACK(base, 12));
		Fset_elt(STACK(base, 10));
		COPY(STACK(base, 4), STACK(base, 9));
		F1plus(STACK(base, 9));
		COPY(STACK(base, 9), STACK(base, 4));
		F1plus(STACK(base, 8));
		goto M147_1;
		RETURN161:;
	}
	Fcdr(STACK(base, 6));
	goto M145_1;
	RETURN159:;
	COPY(STACK(base, 2), STACK(base, 0));
}
