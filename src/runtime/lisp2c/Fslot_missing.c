#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fslot_missing(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&KClisp[14], STACK(base, 4));	/* ~S: The slot ~s is missing from the object ~s of class ~s. */
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 1), STACK(base, 7));
	COPY(STACK(base, 0), STACK(base, 8));
	Ferror(STACK(base, 4), 5);
}
