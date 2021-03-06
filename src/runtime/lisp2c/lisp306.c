/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void Fchar_not_equal(CL_FORM *base, int nargs)
{
	CL_FORM *rest_0;
	CL_FORM *local;
	rest_0 = ARG(1);
	local = ARG(nargs);
	{
		CL_FORM *rest_1;
		rest_1 = rest_0;
		M1_1:;
		if(REST_NOT_EMPTY(rest_1))
		{
		}
		else
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(0));	/* T */
			goto RETURN1;
		}
		{
			CL_FORM *rest_2;
			LOAD_NIL(LOCAL(0));
			rest_2 = rest_1;
			M2_1:;
			if(NOT(REST_NOT_EMPTY(rest_2)))
			{
				LOAD_NIL(LOCAL(0));
				bool_result = CL_TRUEP(LOCAL(0));
				goto RETURN2;
			}
			{
				CL_FORM *rest_3;
				rest_3 = rest_2;
				REST_CAR(rest_3, LOCAL(0));
			}
			{
				if(CL_CHARP(ARG(0)))
				{
					COPY(ARG(0), LOCAL(1));
				}
				else
				{
					COPY(SYMVAL(Slisp, 58), LOCAL(1));	/* WRONG_TYPE */
					COPY(ARG(0), LOCAL(2));
					LOAD_SYMBOL(SYMBOL(Slisp, 18), LOCAL(3));	/* CHARACTER */
					Ferror(LOCAL(1), 3);
				}
				rt_char_upcase(LOCAL(1));
				rt_char_code(LOCAL(1));
			}
			{
				if(CL_CHARP(LOCAL(0)))
				{
					COPY(LOCAL(0), LOCAL(2));
				}
				else
				{
					COPY(SYMVAL(Slisp, 58), LOCAL(2));	/* WRONG_TYPE */
					COPY(LOCAL(0), LOCAL(3));
					LOAD_SYMBOL(SYMBOL(Slisp, 18), LOCAL(4));	/* CHARACTER */
					Ferror(LOCAL(2), 3);
				}
				rt_char_upcase(LOCAL(2));
				rt_char_code(LOCAL(2));
			}
			Fnumneql(LOCAL(1), 2);
			if(CL_TRUEP(LOCAL(1)))
			{
				bool_result = TRUE;
				goto RETURN2;
			}
			{
				CL_FORM *rest_4;
				rest_4 = rest_2;
				rest_2 = REST_CDR(rest_4);
			}
			goto M2_1;
		}
		RETURN2:;
		if(bool_result)
		{
			LOAD_NIL(ARG(0));
			goto RETURN1;
		}
		{
			CL_FORM *rest_5;
			rest_5 = rest_1;
			REST_CAR(rest_5, ARG(0));
		}
		{
			CL_FORM *rest_6;
			rest_6 = rest_1;
			rest_1 = REST_CDR(rest_6);
		}
		goto M1_1;
	}
	RETURN1:;
}
