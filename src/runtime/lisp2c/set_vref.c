#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kset_vref)
{
	MAKE_STRING(27, "~A should be of type VECTOR"),	/* 0 */
};

void set_vref(CL_FORM *base)
{
	if(CL_VECTOR_P(STACK(base, 1)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&Kset_vref[0], STACK(base, 3));	/* ~A should be of type VECTOR */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	Fset_row_major_aref(STACK(base, 0));
}
