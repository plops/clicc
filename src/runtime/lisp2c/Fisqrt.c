#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFisqrt)
{
	MAKE_STRING(48, "Isqrt: ~S argument must be a nonnegative integer"),	/* 0 */
};

void Fisqrt(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fminusp(STACK(base, 1));
		bool_result = NOT(CL_TRUEP(STACK(base, 1)));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Finteger_length(STACK(base, 1));
		LOAD_FIXNUM(1, STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		F1minus(STACK(base, 3));
		LOAD_FIXNUM(-1, STACK(base, 4));
		Fash(STACK(base, 3));
		Fash(STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Foddp(STACK(base, 5));
		if(CL_TRUEP(STACK(base, 5)))
		{
			LOAD_FIXNUM(-1, STACK(base, 5));
		}
		else
		{
			LOAD_FIXNUM(0, STACK(base, 5));
		}
		Fash(STACK(base, 4));
		Fplus(STACK(base, 3), 2);
		M42_1:;
		COPY(STACK(base, 3), STACK(base, 4));
		F1minus(STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fle(STACK(base, 4), 2);
		if(CL_TRUEP(STACK(base, 4)))
		{
			COPY(STACK(base, 2), STACK(base, 0));
			goto RETURN48;
		}
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 5));
		Fplus(STACK(base, 4), 2);
		LOAD_FIXNUM(-1, STACK(base, 5));
		Fash(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 5));
		COPY(STACK(base, 4), STACK(base, 6));
		Fmult(STACK(base, 5), 2);
		COPY(STACK(base, 0), STACK(base, 6));
		Fle(STACK(base, 5), 2);
		if(CL_TRUEP(STACK(base, 5)))
		{
			COPY(STACK(base, 4), STACK(base, 2));
		}
		else
		{
			COPY(STACK(base, 4), STACK(base, 3));
		}
		goto M42_1;
		RETURN48:;
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFisqrt[0], STACK(base, 1));	/* Isqrt: ~S argument must be a nonnegative integer */
		COPY(STACK(base, 0), STACK(base, 2));
		Ferror(STACK(base, 1), 2);
	}
}
