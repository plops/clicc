#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_instance)
{
	MAKE_STRING(113, "The computed symbol ~S is not a valid class-argument, ~\n                   these have been restricted to classes."),	/* 0 */
	MAKE_STRING(33, "~S is not a valid class-argument."),	/* 2 */
};

void Fmake_instance(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 1);
	if(CL_INSTANCEP(STACK(base, 0)))
	{
		LOAD_FIXNUM(-1, STACK(base, 2));
		bool_result = NOT(CL_TRUEP(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 2)))));
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
		if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_instance[0], STACK(base, 2));	/* The computed symbol ~S is not a valid class-argument, ~
                   these have been restricted to classes. */
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_instance[2], STACK(base, 2));	/* ~S is not a valid class-argument. */
		}
		COPY(STACK(base, 0), STACK(base, 3));
		Flist(STACK(base, 3), 1);
		LOAD_SMSTR((CL_FORM *)&KClisp[20], STACK(base, 4));	/* ~&Error in ~A: ~?~% */
		LOAD_SYMBOL(SYMBOL(Slisp, 129), STACK(base, 5));	/* MAKE-INSTANCE */
		COPY(STACK(base, 2), STACK(base, 6));
		COPY(STACK(base, 3), STACK(base, 7));
		Ferror(STACK(base, 4), 4);
	}
	LOAD_FIXNUM(2, STACK(base, 2));
	COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 2))), STACK(base, 2));
	F1plus(STACK(base, 2));
	make_instance_internal(STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_FIXNUM(-1, STACK(base, 4));
	COPY(STACK(base, 0), OFFSET(AR_BASE(GET_FORM(STACK(base, 2))), 1 + GET_FIXNUM(STACK(base, 4))));
	LOAD_NIL(STACK(base, 4));
	LOAD_FIXNUM(3, STACK(base, 5));
	COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 5))), STACK(base, 5));
	M11_1:;
	if(CL_ATOMP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 4));
		goto RETURN11;
	}
	COPY(STACK(base, 5), STACK(base, 6));
	Fcar(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 6));
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
		COPY(STACK(base, 7), OFFSET(AR_BASE(GET_FORM(STACK(base, 2))), 1 + GET_FIXNUM(STACK(base, 3))));
	}
	else
	{
		COPY(STACK(base, 6), OFFSET(AR_BASE(GET_FORM(STACK(base, 2))), 1 + GET_FIXNUM(STACK(base, 3))));
	}
	COPY(STACK(base, 3), STACK(base, 6));
	F1plus(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 3));
	Fcdr(STACK(base, 5));
	goto M11_1;
	RETURN11:;
	COPY(STACK(base, 2), STACK(base, 0));
}
