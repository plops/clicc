#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_list)
{
	MAKE_STRING(32, "make-list: ~A is an illegal size"),	/* 0 */
};

static void Z136_f(CL_FORM *base);

void Fmake_list(CL_FORM *base, int nargs)
{
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 123),	/* INITIAL-ELEMENT */
	};
	keysort(STACK(base, 1), nargs - 1, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	LOAD_FIXNUM(0, STACK(base, 2));
	if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) >= GET_FIXNUM(STACK(base, 2)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_list[0], STACK(base, 2));	/* make-list: ~A is an illegal size */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Z136_f(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z136_f(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	Fzerop(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		F1minus(STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		Z136_f(STACK(base, 2));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 3), 2);
			COPY(STACK(base, 1), CAR(lptr));
			COPY(STACK(base, 2), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
