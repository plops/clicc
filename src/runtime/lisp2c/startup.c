#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kstartup)
{
	MAKE_STRING(7, "KEYWORD"),	/* 0 */
	MAKE_STRREF(&Kstartup[4]),	/* 2 CONS(2) */
	MAKE_NIL,
	MAKE_STRING(0, ""),
};

static void Z162_g1890(CL_FORM *base);
static GLOBAL_FUNARG CZ162_g1890 = {Z162_g1890, 0};

void startup(CL_FORM *base)
{
	LOAD_FIXNUM(6, STACK(base, 0));
	LOAD_SYMBOL(SYMBOL(Slisp, 97), STACK(base, 1));	/* FILL-POINTER */
	LOAD_FIXNUM(0, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 96), STACK(base, 3));	/* ADJUSTABLE */
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	Fmake_array(STACK(base, 0), 5);
	COPY(STACK(base, 0), SYMVAL(Slisp, 302));	/* *PACKAGE-ARRAY* */
	LOAD_SMSTR((CL_FORM *)&Kstartup[0], STACK(base, 0));	/* KEYWORD */
	LOAD_SYMBOL(SYMBOL(Slisp, 304), STACK(base, 1));	/* NICKNAMES */
	LOAD_CONS((CL_FORM *)&Kstartup[2], STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 311), STACK(base, 3));	/* USE */
	LOAD_NIL(STACK(base, 4));
	Fmake_package(STACK(base, 0), 5);
	COPY(STACK(base, 0), SYMVAL(Slisp, 303));	/* *KEYWORD-PACKAGE* */
	LOAD_SYMBOL(SYMBOL(Slisp, 437), STACK(base, 0));	/* ERROR-CATCHER */
	LOAD_GLOBFUN(&CZ162_g1890, STACK(base, 1));
	catch_internal(STACK(base, 0));
}

static void Z162_g1890(CL_FORM *base)
{
	Imain(STACK(base, 0));
	LOAD_FIXNUM(0, STACK(base, 0));
}
