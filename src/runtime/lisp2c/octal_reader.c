#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Coctal_reader = {octal_reader, 3};

void octal_reader(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[4], STACK(base, 3));	/* extra argument for #~S */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	LOAD_FIXNUM(8, STACK(base, 2));
	radix_reader(STACK(base, 0));
}
