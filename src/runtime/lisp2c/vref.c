#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kvref)
{
	MAKE_STRING(27, "~A should be of type VECTOR"),	/* 0 */
};

void vref(CL_FORM *base)
{
	if(CL_VECTOR_P(STACK(base, 0)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&Kvref[0], STACK(base, 2));	/* ~A should be of type VECTOR */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	Frow_major_aref(STACK(base, 0));
}
