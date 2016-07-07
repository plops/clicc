#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cclose1 = {close1, 1};

CL_INIT2(Kclose1)
{
	MAKE_STRING(14, "already closed"),	/* 0 */
};

static void Z1_g89(CL_FORM *base);
static GLOBAL_FUNARG CZ1_g89 = {Z1_g89, 1};

void close1(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(2, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
	LOAD_GLOBFUN(&Cclose_err, STACK(base, 5));
	set_struct_ref(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(3, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
	LOAD_GLOBFUN(&Cclose_err, STACK(base, 5));
	set_struct_ref(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(4, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
	LOAD_GLOBFUN(&Cclose_err, STACK(base, 5));
	set_struct_ref(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(6, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
	LOAD_GLOBFUN(&Cclose_err, STACK(base, 5));
	set_struct_ref(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(7, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
	LOAD_GLOBFUN(&Cclose_err, STACK(base, 5));
	set_struct_ref(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_GLOBFUN(&CZ1_g89, STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_FIXNUM(9, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 5));	/* STREAM */
	COPY(STACK(base, 2), STACK(base, 6));
	set_struct_ref(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 0));
}

static void Z1_g89(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&Kclose1[0], STACK(base, 0));	/* already closed */
}
