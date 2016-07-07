#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fslot_boundp(CL_FORM *base)
{
	if(CL_INSTANCEP(STACK(base, 0)))
	{
		LOAD_FIXNUM(-1, STACK(base, 2));
		COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 2))), STACK(base, 2));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[18], STACK(base, 2));	/* ~S ist not a valid argument for CLASS-OF, ~
              these have been restricted to instances of user-defined-classes. */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	LOAD_FIXNUM(3, STACK(base, 3));
	COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 2))), 1 + GET_FIXNUM(STACK(base, 3))), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 5));	/* KEY */
	LOAD_GLOBFUN(&CFthird, STACK(base, 6));
	Fposition(STACK(base, 3), 4);
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 3))), STACK(base, 4));
		unbound_value_p(STACK(base, 4));
		if(NOT(CL_TRUEP(STACK(base, 4))))
		{
			LOAD_T(STACK(base, 0));
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
	else
	{
		if(CL_INSTANCEP(STACK(base, 0)))
		{
			LOAD_FIXNUM(-1, STACK(base, 4));
			COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 4))), STACK(base, 4));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[18], STACK(base, 4));	/* ~S ist not a valid argument for CLASS-OF, ~
              these have been restricted to instances of user-defined-classes. */
			COPY(STACK(base, 0), STACK(base, 5));
			Ferror(STACK(base, 4), 2);
		}
		LOAD_SMSTR((CL_FORM *)&KClisp[14], STACK(base, 5));	/* ~S: The slot ~s is missing from the object ~s of class ~s. */
		LOAD_SYMBOL(SYMBOL(Slisp, 131), STACK(base, 6));	/* SLOT-BOUNDP */
		COPY(STACK(base, 1), STACK(base, 7));
		COPY(STACK(base, 0), STACK(base, 8));
		COPY(STACK(base, 4), STACK(base, 9));
		Ferror(STACK(base, 5), 5);
	}
}
