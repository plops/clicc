#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFinput_stream_p)
{
	MAKE_SYMREF(SYMBOL(Slisp, 100)),	/* 0 CONS(0) FILE-INPUT */
	MAKE_LIST(&KFinput_stream_p[2]),
	MAKE_SYMREF(SYMBOL(Slisp, 101)),	/* CONS(2) FILE-IO */
	MAKE_LIST(&KFinput_stream_p[4]),
	MAKE_SYMREF(SYMBOL(Slisp, 91)),	/* CONS(4) CONCATENATED */
	MAKE_LIST(&KFinput_stream_p[6]),
	MAKE_SYMREF(SYMBOL(Slisp, 92)),	/* CONS(6) TWO-WAY */
	MAKE_LIST(&KFinput_stream_p[8]),
	MAKE_SYMREF(SYMBOL(Slisp, 93)),	/* CONS(8) ECHO */
	MAKE_LIST(&KFinput_stream_p[10]),
	MAKE_SYMREF(SYMBOL(Slisp, 94)),	/* CONS(10) STRING-INPUT */
	MAKE_NIL,
};

void Finput_stream_p(CL_FORM *base)
{
	M2_1:;
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
		goto M2_1;
	}
	else
	{
		LOAD_FIXNUM(0, STACK(base, 1));
		LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 2));	/* STREAM */
		struct_ref(STACK(base, 0));
		LOAD_CONS((CL_FORM *)&KFinput_stream_p[0], STACK(base, 1));
		Fmember(STACK(base, 0), 2);
	}
	goto RETURN2;
	RETURN2:;
}
