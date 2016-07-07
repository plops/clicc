#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fslot_unbound(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&KClisp[16], STACK(base, 3));	/* The slot ~s is unbound in the object ~s of class ~s. */
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 6));
	Ferror(STACK(base, 3), 4);
}
