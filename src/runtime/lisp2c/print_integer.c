#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z94_f(CL_FORM *base, CL_FORM *display[]);

void print_integer(CL_FORM *base)
{
	CL_FORM *display[1];
	if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 0)
	{
		LOAD_CHAR('0', STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		Fwrite_char(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fminusp(STACK(base, 2));
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_CHAR('-', STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 3));
			Fwrite_char(STACK(base, 2), 2);
			mv_count = 1;
			COPY(STACK(base, 0), STACK(base, 2));
			COPY(SYMVAL(Slisp, 400), STACK(base, 3));	/* *PRINT-BASE* */
			truncate_internal(STACK(base, 2));
			Fminus(STACK(base, 2), 1);
			display[0] = STACK(base, 0);
			Z94_f(STACK(base, 2), display);
			mv_count = 1;
			COPY(STACK(base, 0), STACK(base, 2));
			COPY(SYMVAL(Slisp, 400), STACK(base, 3));	/* *PRINT-BASE* */
			Frem(STACK(base, 2));
			Fminus(STACK(base, 2), 1);
			Fdigit_char(STACK(base, 2), 1);
			COPY(STACK(base, 1), STACK(base, 3));
			Fwrite_char(STACK(base, 2), 2);
			COPY(STACK(base, 2), STACK(base, 0));
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 2));
			display[0] = STACK(base, 0);
			Z94_f(STACK(base, 2), display);
			COPY(STACK(base, 2), STACK(base, 0));
		}
	}
}

static void Z94_f(CL_FORM *base, CL_FORM *display[])
{
	if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 0)
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		COPY(SYMVAL(Slisp, 400), STACK(base, 2));	/* *PRINT-BASE* */
		truncate_internal(STACK(base, 1));
		Z94_f(STACK(base, 1), display);
		mv_count = 1;
		COPY(SYMVAL(Slisp, 400), STACK(base, 1));	/* *PRINT-BASE* */
		Frem(STACK(base, 0));
		Fdigit_char(STACK(base, 0), 1);
		COPY(&display[0][1], STACK(base, 1));
		Fwrite_char(STACK(base, 0), 2);
	}
}
