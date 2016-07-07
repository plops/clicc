#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void the_type_error(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&KClisp[8], STACK(base, 2));	/* type error: ~S is not of type ~S */
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Ferror(STACK(base, 2), 3);
}
