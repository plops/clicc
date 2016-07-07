#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFset_slot_value)
{
	MAKE_SYMREF(SYMBOL(Slisp, 140)),	/* 0 CONS(0) SETF */
	MAKE_LIST(&KFset_slot_value[2]),
	MAKE_SYMREF(SYMBOL(Slisp, 130)),	/* CONS(2) SLOT-VALUE */
	MAKE_NIL,
};

void Fset_slot_value(CL_FORM *base)
{
	if(CL_INSTANCEP(STACK(base, 1)))
	{
		LOAD_FIXNUM(-1, STACK(base, 3));
		COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 1))), 1 + GET_FIXNUM(STACK(base, 3))), STACK(base, 3));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[18], STACK(base, 3));	/* ~S ist not a valid argument for CLASS-OF, ~
              these have been restricted to instances of user-defined-classes. */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	LOAD_FIXNUM(3, STACK(base, 4));
	COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 3))), 1 + GET_FIXNUM(STACK(base, 4))), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 6));	/* KEY */
	LOAD_GLOBFUN(&CFthird, STACK(base, 7));
	Fposition(STACK(base, 4), 4);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 0), OFFSET(AR_BASE(GET_FORM(STACK(base, 1))), 1 + GET_FIXNUM(STACK(base, 4))));
	}
	if(CL_INSTANCEP(STACK(base, 1)))
	{
		LOAD_FIXNUM(-1, STACK(base, 5));
		COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 1))), 1 + GET_FIXNUM(STACK(base, 5))), STACK(base, 5));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[18], STACK(base, 5));	/* ~S ist not a valid argument for CLASS-OF, ~
              these have been restricted to instances of user-defined-classes. */
		COPY(STACK(base, 1), STACK(base, 6));
		Ferror(STACK(base, 5), 2);
	}
	LOAD_CONS((CL_FORM *)&KFset_slot_value[0], STACK(base, 6));
	LOAD_SMSTR((CL_FORM *)&KClisp[14], STACK(base, 7));	/* ~S: The slot ~s is missing from the object ~s of class ~s. */
	COPY(STACK(base, 6), STACK(base, 8));
	COPY(STACK(base, 2), STACK(base, 9));
	COPY(STACK(base, 1), STACK(base, 10));
	COPY(STACK(base, 5), STACK(base, 11));
	Ferror(STACK(base, 7), 5);
}
