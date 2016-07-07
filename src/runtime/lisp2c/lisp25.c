#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFoutput_stream_p)
{
	MAKE_SYMREF(SYMBOL(Slisp, 102)),	/* 0 CONS(0) FILE-OUTPUT */
	MAKE_LIST(&KFoutput_stream_p[2]),
	MAKE_SYMREF(SYMBOL(Slisp, 101)),	/* CONS(2) FILE-IO */
	MAKE_LIST(&KFoutput_stream_p[4]),
	MAKE_SYMREF(SYMBOL(Slisp, 90)),	/* CONS(4) BROADCAST */
	MAKE_LIST(&KFoutput_stream_p[6]),
	MAKE_SYMREF(SYMBOL(Slisp, 92)),	/* CONS(6) TWO-WAY */
	MAKE_LIST(&KFoutput_stream_p[8]),
	MAKE_SYMREF(SYMBOL(Slisp, 93)),	/* CONS(8) ECHO */
	MAKE_LIST(&KFoutput_stream_p[10]),
	MAKE_SYMREF(SYMBOL(Slisp, 98)),	/* CONS(10) STRING-OUTPUT */
	MAKE_NIL,
};

void Foutput_stream_p(CL_FORM *base)
{
	M3_1:;
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_FIXNUM(0, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 3));	/* STREAM */
	struct_ref(STACK(base, 1));
	if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 89))	/* SYNONYM */
	{
		COPY(STACK(base, 0), STACK(base, 1));
		LOAD_FIXNUM(1, STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 3));	/* STREAM */
		struct_ref(STACK(base, 1));
		Fsymbol_value(STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 0));
		goto M3_1;
	}
	else
	{
		LOAD_FIXNUM(0, STACK(base, 1));
		LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 2));	/* STREAM */
		struct_ref(STACK(base, 0));
		LOAD_CONS((CL_FORM *)&KFoutput_stream_p[0], STACK(base, 1));
		Fmember(STACK(base, 0), 2);
	}
	goto RETURN3;
	RETURN3:;
}
