#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFfind_symbol)
{
	MAKE_STRING(18, "~a is not a string"),	/* 0 */
};

void Ffind_symbol(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 272), STACK(base, 1));	/* *PACKAGE* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_STRING_P(STACK(base, 0)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFfind_symbol[0], STACK(base, 2));	/* ~a is not a string */
		COPY(STACK(base, 0), STACK(base, 3));
		Ferror(STACK(base, 2), 2);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 3));
	string_hash(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	coerce_to_package(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 1));
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(3, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 6));	/* PACKAGE */
	struct_ref(STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fsvref(STACK(base, 4));
	M49_1:;
	if(CL_ATOMP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 3));
		goto RETURN57;
	}
	COPY(STACK(base, 4), STACK(base, 5));
	Fcar(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fsymbol_name(STACK(base, 6));
	FstringE(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 3), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Slisp, 306), STACK(base, 6));	/* EXTERNAL */
		COPY(STACK(base, 5), STACK(base, 0));
		COPY(STACK(base, 6), &mv_buf[0]);
		mv_count = 2;
		goto RETURN56;
	}
	Fcdr(STACK(base, 4));
	goto M49_1;
	RETURN57:;
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(2, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 6));	/* PACKAGE */
	struct_ref(STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fsvref(STACK(base, 4));
	M50_1:;
	if(CL_ATOMP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 3));
		goto RETURN58;
	}
	COPY(STACK(base, 4), STACK(base, 5));
	Fcar(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fsymbol_name(STACK(base, 6));
	FstringE(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 3), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Slisp, 305), STACK(base, 6));	/* INTERNAL */
		COPY(STACK(base, 5), STACK(base, 0));
		COPY(STACK(base, 6), &mv_buf[0]);
		mv_count = 2;
		goto RETURN56;
	}
	Fcdr(STACK(base, 4));
	goto M50_1;
	RETURN58:;
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(5, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 6));	/* PACKAGE */
	struct_ref(STACK(base, 4));
	M51_1:;
	if(CL_ATOMP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 3));
		goto RETURN59;
	}
	COPY(STACK(base, 4), STACK(base, 5));
	Fcar(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 3));
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	LOAD_FIXNUM(3, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
	struct_ref(STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	Fsvref(STACK(base, 6));
	M52_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN60;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Fsymbol_name(STACK(base, 8));
	FstringE(STACK(base, 7), 2);
	if(CL_TRUEP(STACK(base, 7)))
	{
		COPY(STACK(base, 5), STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 312), STACK(base, 8));	/* INHERITED */
		COPY(STACK(base, 7), STACK(base, 0));
		COPY(STACK(base, 8), &mv_buf[0]);
		mv_count = 2;
		goto RETURN56;
	}
	Fcdr(STACK(base, 6));
	goto M52_1;
	RETURN60:;
	Fcdr(STACK(base, 4));
	goto M51_1;
	RETURN59:;
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 0));
	COPY(STACK(base, 4), &mv_buf[0]);
	mv_count = 2;
	RETURN56:;
}
