#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_package)
{
	MAKE_STRREF(&KFmake_package[2]),	/* 0 CONS(0) */
	MAKE_NIL,
	MAKE_STRING(4, "LISP"),
	MAKE_STRING(35, "package with name ~S already exists"),	/* 2 */
};

void Fmake_package(CL_FORM *base, int nargs)
{
	BOOL supl_flags[2];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 304),	/* NICKNAMES */
		SYMBOL(Slisp, 311),	/* USE */
	};
	keysort(STACK(base, 1), nargs - 1, 2, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_CONS((CL_FORM *)&KFmake_package[0], STACK(base, 2));
	}
	LOAD_NIL(STACK(base, 3));
	if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 4));
		Fsymbol_name(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 0));
	}
	COPY(STACK(base, 0), STACK(base, 4));
	Ffind_package(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_package[4], STACK(base, 4));	/* package with name ~S already exists */
		COPY(STACK(base, 0), STACK(base, 5));
		Ferror(STACK(base, 4), 2);
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 185), STACK(base, 3));	/* NAME */
	COPY(STACK(base, 0), STACK(base, 4));
	raw_make_package(STACK(base, 3), 2);
	COPY(STACK(base, 3), STACK(base, 4));
	COPY(SYMVAL(Slisp, 302), STACK(base, 5));	/* *PACKAGE-ARRAY* */
	Fvector_push_extend(STACK(base, 4), 2);
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	Fuse_package(STACK(base, 4), 2);
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	add_nicknames(STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 0));
}
