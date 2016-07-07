#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFreinitialize_instance)
{
	MAKE_STRING(22, "~S is not an instance."),	/* 0 */
};

void Freinitialize_instance(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 2), nargs - 2);
	if(NOT(CL_INSTANCEP(STACK(base, 0))))
	{
		LOAD_T(STACK(base, 3));
	}
	else
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(CL_TRUEP(STACK(base, 3)))
	{
		bool_result = TRUE;
	}
	else
	{
		LOAD_FIXNUM(-1, STACK(base, 4));
		bool_result = CL_TRUEP(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 4))));
	}
	if(bool_result)
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFreinitialize_instance[0], STACK(base, 3));	/* ~S is not an instance. */
		COPY(STACK(base, 0), STACK(base, 4));
		Flist(STACK(base, 4), 1);
		LOAD_SMSTR((CL_FORM *)&KClisp[20], STACK(base, 5));	/* ~&Error in ~A: ~?~% */
		LOAD_SYMBOL(SYMBOL(Slisp, 138), STACK(base, 6));	/* REINITIALIZE-INSTANCE */
		COPY(STACK(base, 3), STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 8));
		Ferror(STACK(base, 5), 4);
	}
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	LOAD_FIXNUM(3, STACK(base, 5));
	COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 5))), STACK(base, 5));
	M12_1:;
	if(CL_ATOMP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 0));
		goto RETURN12;
	}
	COPY(STACK(base, 5), STACK(base, 6));
	Fcar(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fcar(STACK(base, 7));
	get_prop(STACK(base, 6));
	if(CL_TRUEP(STACK(base, 6)))
	{
		Fcdr(STACK(base, 6));
		Fcar(STACK(base, 6));
	}
	else
	{
		COPY(STACK(base, 4), STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 6));
		Fcdr(STACK(base, 6));
		Fcar(STACK(base, 6));
	}
	COPY(STACK(base, 4), STACK(base, 7));
	COPY(STACK(base, 4), STACK(base, 7));
	Fcdr(STACK(base, 7));
	Fcar(STACK(base, 7));
	if(EQ(STACK(base, 6), STACK(base, 7)))
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Ssys, 1))	/* T */
		{
			LOAD_T(STACK(base, 7));
		}
		else
		{
			LOAD_NIL(STACK(base, 7));
		}
		if(CL_TRUEP(STACK(base, 7)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 4), STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 8));
			Fcdr(STACK(base, 8));
			Fcdr(STACK(base, 8));
			Fcar(STACK(base, 8));
			COPY(STACK(base, 2), STACK(base, 9));
			Fmember(STACK(base, 8), 2);
			bool_result = CL_TRUEP(STACK(base, 8));
		}
		if(bool_result)
		{
			if(CL_FUNCTION_P(STACK(base, 6)))
			{
				COPY(STACK(base, 4), STACK(base, 7));
				COPY(STACK(base, 4), STACK(base, 7));
				Fcdr(STACK(base, 7));
				Fcar(STACK(base, 7));
				bool_result = EQ(STACK(base, 6), STACK(base, 7));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 6), STACK(base, 7));
				Ffuncall(STACK(base, 7), 1);
				mv_count = 1;
				COPY(STACK(base, 7), OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 3))));
			}
			else
			{
				COPY(STACK(base, 6), OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 3))));
			}
		}
	}
	else
	{
		if(CL_FUNCTION_P(STACK(base, 6)))
		{
			COPY(STACK(base, 4), STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 7));
			Fcdr(STACK(base, 7));
			Fcar(STACK(base, 7));
			bool_result = EQ(STACK(base, 6), STACK(base, 7));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 6), STACK(base, 7));
			Ffuncall(STACK(base, 7), 1);
			mv_count = 1;
			COPY(STACK(base, 7), OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 3))));
		}
		else
		{
			COPY(STACK(base, 6), OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 3))));
		}
	}
	COPY(STACK(base, 3), STACK(base, 6));
	F1plus(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 3));
	Fcdr(STACK(base, 5));
	goto M12_1;
	RETURN12:;
}
