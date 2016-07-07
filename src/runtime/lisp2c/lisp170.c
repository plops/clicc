#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Craw_make_package = {raw_make_package, -1};

CL_INIT2(Kraw_make_package)
{
	MAKE_STRING(0, ""),	/* 0 */
};

void raw_make_package(CL_FORM *base, int nargs)
{
	BOOL supl_flags[7];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 185),	/* NAME */
		SYMBOL(Slisp, 304),	/* NICKNAMES */
		SYMBOL(Slisp, 305),	/* INTERNAL */
		SYMBOL(Slisp, 306),	/* EXTERNAL */
		SYMBOL(Slisp, 307),	/* SHADOWINGS */
		SYMBOL(Slisp, 308),	/* USE-LIST */
		SYMBOL(Slisp, 309),	/* USED-BY-LIST */
	};
	keysort(STACK(base, 0), nargs - 0, 7, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_SMSTR((CL_FORM *)&Kraw_make_package[0], STACK(base, 0));	/*  */
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_FIXNUM(101, STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 8));	/* INITIAL-ELEMENT */
		LOAD_NIL(STACK(base, 9));
		LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 10));	/* ELEMENT-TYPE */
		LOAD_SYMBOL(SYMBOL(Slisp, 28), STACK(base, 11));	/* LIST */
		Fmake_array(STACK(base, 7), 5);
		COPY(STACK(base, 7), STACK(base, 2));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_FIXNUM(101, STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 8));	/* INITIAL-ELEMENT */
		LOAD_NIL(STACK(base, 9));
		LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 10));	/* ELEMENT-TYPE */
		LOAD_SYMBOL(SYMBOL(Slisp, 28), STACK(base, 11));	/* LIST */
		Fmake_array(STACK(base, 7), 5);
		COPY(STACK(base, 7), STACK(base, 3));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_NIL(STACK(base, 6));
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
	COPY(STACK(base, 0), STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	COPY(STACK(base, 2), STACK(base, 10));
	COPY(STACK(base, 3), STACK(base, 11));
	COPY(STACK(base, 4), STACK(base, 12));
	COPY(STACK(base, 5), STACK(base, 13));
	COPY(STACK(base, 6), STACK(base, 14));
	make_struct(STACK(base, 7), 8);
	COPY(STACK(base, 7), STACK(base, 0));
}
