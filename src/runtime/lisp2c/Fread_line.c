#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFread_line)
{
	MAKE_STRING(22, "unexpected end of file"),	/* 0 */
};

void Fread_line(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		LOAD_NIL(STACK(base, 0));
		case 1:
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		LOAD_NIL(STACK(base, 3));
		case 4:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Ssys, 1))	/* T */
		{
			COPY(SYMVAL(Slisp, 58), STACK(base, 0));	/* *TERMINAL-IO* */
		}
	}
	else
	{
		COPY(SYMVAL(Slisp, 59), STACK(base, 0));	/* *STANDARD-INPUT* */
	}
	COPY(STACK(base, 0), STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	Fread_char(STACK(base, 4), 3);
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_FIXNUM(0, STACK(base, 5));
		COPY(SYMVAL(Slisp, 376), STACK(base, 6));	/* *TOKEN* */
		Fset_fill_pointer(STACK(base, 5));
		M100_1:;
		if(CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '\n')
		{
			COPY(SYMVAL(Slisp, 376), STACK(base, 5));	/* *TOKEN* */
			LOAD_FIXNUM(0, STACK(base, 6));
			Fsubseq(STACK(base, 5), 2);
			LOAD_NIL(STACK(base, 6));
			COPY(STACK(base, 5), STACK(base, 0));
			COPY(STACK(base, 6), &mv_buf[0]);
			mv_count = 2;
			goto RETURN112;
		}
		COPY(STACK(base, 4), STACK(base, 5));
		COPY(SYMVAL(Slisp, 376), STACK(base, 6));	/* *TOKEN* */
		Fvector_push_extend(STACK(base, 5), 2);
		COPY(STACK(base, 0), STACK(base, 4));
		LOAD_NIL(STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
		Fread_char(STACK(base, 4), 3);
		if(CL_TRUEP(STACK(base, 4)))
		{
		}
		else
		{
			COPY(SYMVAL(Slisp, 376), STACK(base, 5));	/* *TOKEN* */
			LOAD_FIXNUM(0, STACK(base, 6));
			Fsubseq(STACK(base, 5), 2);
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
			COPY(STACK(base, 5), STACK(base, 0));
			COPY(STACK(base, 6), &mv_buf[0]);
			mv_count = 2;
			goto RETURN112;
		}
		goto M100_1;
		RETURN112:;
	}
	else
	{
		if(CL_TRUEP(STACK(base, 1)))
		{
			LOAD_SMSTR((CL_FORM *)&KFread_line[0], STACK(base, 5));	/* unexpected end of file */
			Ferror(STACK(base, 5), 1);
		}
		COPY(STACK(base, 2), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		COPY(STACK(base, 5), STACK(base, 0));
		COPY(STACK(base, 6), &mv_buf[0]);
		mv_count = 2;
	}
}
