#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cunparse_unix_namestring = {unparse_unix_namestring, 1};

void unparse_unix_namestring(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 1));	/* SIMPLE-STRING */
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(2, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 4));	/* PATHNAME */
	struct_ref(STACK(base, 2));
	unparse_unix_directory_list(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	unparse_unix_file(STACK(base, 3));
	Fconcatenate(STACK(base, 1), 3);
	COPY(STACK(base, 1), STACK(base, 0));
}
