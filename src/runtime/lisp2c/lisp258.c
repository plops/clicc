#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Csemicolon_reader = {semicolon_reader, 2};

void semicolon_reader(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	Fread_line(STACK(base, 2), 4);
	mv_count = 1;
	LOAD_NIL(STACK(base, 0));
	mv_count = 0;
}
