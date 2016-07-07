#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kcoerce_to_package)
{
	MAKE_STRING(32, "there is no Package with name ~S"),	/* 0 */
};

void coerce_to_package(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
	{
		Fsymbol_name(STACK(base, 0));
	}
	if(CL_STRING_P(STACK(base, 0)))
	{
		Ffind_package(STACK(base, 0));
	}
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 2));	/* PACKAGE */
	struct_typep(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&Kcoerce_to_package[0], STACK(base, 1));	/* there is no Package with name ~S */
		COPY(STACK(base, 0), STACK(base, 2));
		Ferror(STACK(base, 1), 2);
	}
}
