#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cunparse_unix_host = {unparse_unix_host, 1};

CL_INIT2(Kunparse_unix_host)
{
	MAKE_STRING(4, "Unix"),	/* 0 */
};

void unparse_unix_host(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&Kunparse_unix_host[0], STACK(base, 0));	/* Unix */
}
