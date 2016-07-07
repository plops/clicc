#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFrename_package)
{
	MAKE_STRING(35, "package with name ~S already exists"),	/* 0 */
};

void Frename_package(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_SYMBOLP(STACK(base, 1)) || CL_NILP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 3));
		Fsymbol_name(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 1));
	}
	COPY(STACK(base, 0), STACK(base, 3));
	coerce_to_package(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 0));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffind_package(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		bool_result = NOT(EQ(STACK(base, 0), STACK(base, 3)));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_SMSTR((CL_FORM *)&KFrename_package[0], STACK(base, 4));	/* package with name ~S already exists */
		Ferror(STACK(base, 4), 1);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	LOAD_FIXNUM(0, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
	COPY(STACK(base, 1), STACK(base, 8));
	set_struct_ref(STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	LOAD_FIXNUM(1, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 6));	/* PACKAGE */
	LOAD_NIL(STACK(base, 7));
	set_struct_ref(STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	add_nicknames(STACK(base, 3));
}
