#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kcheck_seq_count)
{
	MAKE_STRING(60, "~S should be an INTEGER at least 0 and no more than 16777214"),	/* 0 */
};

void check_seq_count(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fplusp(STACK(base, 2));
		if(CL_TRUEP(STACK(base, 2)))
		{
		}
		else
		{
			LOAD_FIXNUM(0, STACK(base, 0));
		}
	}
	else
	{
		if(CL_TRUEP(STACK(base, 0)))
		{
			LOAD_SMSTR((CL_FORM *)&Kcheck_seq_count[0], STACK(base, 2));	/* ~S should be an INTEGER at least 0 and no more than 16777214 */
			COPY(STACK(base, 0), STACK(base, 3));
			Ferror(STACK(base, 2), 2);
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 0));
		}
	}
}
