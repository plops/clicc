#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFformat)
{
	MAKE_STRING(33, "illegal destination ~S for format"),	/* 0 */
};

static void Z96_g1182(CL_FORM *base);
static void Z97_g1181(CL_FORM *base);

void Fformat(CL_FORM *base, int nargs)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 0 + nargs));
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 0 + nargs));
	Flist(STACK(base, 2), nargs - 2);
	GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
	LOAD_NIL(STACK(base, 3));
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 4));
	if(NOT(CL_TRUEP(INDIRECT(STACK(base, 0)))))
	{
		LOAD_T(STACK(base, 4));
	}
	else
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(CL_TRUEP(STACK(base, 4)))
	{
		bool_result = TRUE;
	}
	else
	{
		bool_result = CL_STRING_P(INDIRECT(STACK(base, 0)));
	}
	if(bool_result)
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 0))))
		{
			COPY(INDIRECT(STACK(base, 0)), STACK(base, 4));
			Fmake_string_output_stream(STACK(base, 4), 1);
			COPY(STACK(base, 4), INDIRECT(STACK(base, 3)));
		}
		else
		{
			Fmake_string_output_stream(STACK(base, 4), 0);
			COPY(STACK(base, 4), INDIRECT(STACK(base, 3)));
		}
		{
			GEN_CLOSURE(array, STACK(base, 4), 7, Z97_g1181, 0);
			COPY(STACK(base, 0), &array[3]);
			COPY(STACK(base, 2), &array[4]);
			COPY(STACK(base, 1), &array[5]);
			COPY(STACK(base, 3), &array[6]);
			LOAD_CLOSURE(array, STACK(base, 4));
		}
		COPY(STACK(base, 4), STACK(base, 4));
		{
			GEN_CLOSURE(array, STACK(base, 5), 4, Z96_g1182, 0);
			COPY(STACK(base, 3), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 5));
		}
		COPY(STACK(base, 5), STACK(base, 5));
		unwind_protect_internal(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 0));
	}
	else
	{
		if(CL_SYMBOLP(INDIRECT(STACK(base, 0))) && GET_SYMBOL(INDIRECT(STACK(base, 0))) == SYMBOL(Ssys, 1))	/* T */
		{
			COPY(SYMVAL(Slisp, 60), STACK(base, 4));	/* *STANDARD-OUTPUT* */
			COPY(INDIRECT(STACK(base, 1)), STACK(base, 5));
			COPY(INDIRECT(STACK(base, 2)), STACK(base, 6));
			format2(STACK(base, 4), 3);
			mv_count = 1;
			LOAD_NIL(STACK(base, 0));
		}
		else
		{
			COPY(INDIRECT(STACK(base, 0)), STACK(base, 4));
			Foutput_stream_p(STACK(base, 4));
			if(CL_TRUEP(STACK(base, 4)))
			{
				COPY(INDIRECT(STACK(base, 0)), STACK(base, 4));
				COPY(INDIRECT(STACK(base, 1)), STACK(base, 5));
				COPY(INDIRECT(STACK(base, 2)), STACK(base, 6));
				format2(STACK(base, 4), 3);
				mv_count = 1;
				LOAD_NIL(STACK(base, 0));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFformat[0], STACK(base, 4));	/* illegal destination ~S for format */
				COPY(INDIRECT(STACK(base, 0)), STACK(base, 5));
				Ferror(STACK(base, 4), 2);
			}
		}
	}
}

static void Z96_g1182(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	stream_close(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z97_g1181(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 6), STACK(base, 1));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 3));
	format2(STACK(base, 1), 3);
	mv_count = 1;
	if(CL_TRUEP(INDIRECT(GET_FORM(STACK(base, 0)) + 3)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 6), STACK(base, 1));
		Fget_output_stream_string(STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 0));
	}
}
