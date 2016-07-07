#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cunparse_unix_directory = {unparse_unix_directory, 1};

void unparse_unix_directory(CL_FORM *base)
{
	LOAD_FIXNUM(2, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 2));	/* PATHNAME */
	struct_ref(STACK(base, 0));
	unparse_unix_directory_list(STACK(base, 0));
}
