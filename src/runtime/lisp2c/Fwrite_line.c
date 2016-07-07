#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fwrite_line(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	LOAD_GLOBFUN(&CFwrite_string, STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	Fapply(STACK(base, 1), 2);
	mv_count = 1;
	Fterpri(STACK(base, 2), 0);
	COPY(STACK(base, 1), STACK(base, 0));
}
