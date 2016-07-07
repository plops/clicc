#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fclass_of(CL_FORM *base)
{
	if(CL_INSTANCEP(STACK(base, 0)))
	{
		LOAD_FIXNUM(-1, STACK(base, 1));
		COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 1))), STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[18], STACK(base, 1));	/* ~S ist not a valid argument for CLASS-OF, ~
              these have been restricted to instances of user-defined-classes. */
		COPY(STACK(base, 0), STACK(base, 2));
		Ferror(STACK(base, 1), 2);
	}
}
