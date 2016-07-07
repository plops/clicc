#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kfile_name)
{
	MAKE_SYMREF(SYMBOL(Slisp, 100)),	/* 0 CONS(0) FILE-INPUT */
	MAKE_LIST(&Kfile_name[2]),
	MAKE_SYMREF(SYMBOL(Slisp, 102)),	/* CONS(2) FILE-OUTPUT */
	MAKE_LIST(&Kfile_name[4]),
	MAKE_SYMREF(SYMBOL(Slisp, 101)),	/* CONS(4) FILE-IO */
	MAKE_NIL,
	MAKE_STRING(46, "there is no filename associated with stream ~a"),	/* 2 */
};

void file_name(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
	struct_ref(STACK(base, 2));
	LOAD_CONS((CL_FORM *)&Kfile_name[0], STACK(base, 3));
	Fmember(STACK(base, 2), 2);
	if(CL_TRUEP(STACK(base, 2)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&Kfile_name[6], STACK(base, 2));	/* there is no filename associated with stream ~a */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		LOAD_FIXNUM(1, STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 6));	/* STREAM */
		COPY(STACK(base, 1), STACK(base, 7));
		set_struct_ref(STACK(base, 4));
	}
	LOAD_FIXNUM(1, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 2));	/* STREAM */
	struct_ref(STACK(base, 0));
}
