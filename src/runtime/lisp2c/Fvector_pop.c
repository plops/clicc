#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFvector_pop)
{
	MAKE_STRING(42, "The fill pointer of the vector ~A is zero."),	/* 0 */
};

void Fvector_pop(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Ffill_pointer(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	Fzerop(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KFvector_pop[0], STACK(base, 2));	/* The fill pointer of the vector ~A is zero. */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	F1minus(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	Fset_fill_pointer(STACK(base, 2));
	vref(STACK(base, 0));
}
