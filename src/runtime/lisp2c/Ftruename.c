#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFtruename)
{
	MAKE_STRING(27, "The file ~S does not exist."),	/* 0 */
};

void Ftruename(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Fprobe_file(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFtruename[0], STACK(base, 2));	/* The file ~S does not exist. */
		COPY(STACK(base, 0), STACK(base, 3));
		Fnamestring(STACK(base, 3));
		mv_count = 1;
		Ferror(STACK(base, 2), 2);
	}
	COPY(STACK(base, 1), STACK(base, 0));
}
