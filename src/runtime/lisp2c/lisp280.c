#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z92_g1100(CL_FORM *base);
static void Z93_g1096(CL_FORM *base);

void Fread_from_string(CL_FORM *base, int nargs)
{
	if(nargs > 3)
	{
		goto ALL_OPT_SUPPLIED;
	}
	switch(nargs)
	{
		case 3:
		ALL_OPT_SUPPLIED:;
		GEN_HEAPVAR(STACK(base, 2), STACK(base, 0 + nargs));
		case 2:
		GEN_HEAPVAR(STACK(base, 1), STACK(base, 0 + nargs));
	}
	switch(nargs)
	{
		case 1:
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
		GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
		case 2:
		LOAD_NIL(STACK(base, 2));
		GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
		nargs = 3;
	}
	{
		BOOL supl_flags[3];
		static CL_FORM * keylist[] =
		{
			SYMBOL(Slisp, 161),	/* START */
			SYMBOL(Slisp, 162),	/* END */
			SYMBOL(Slisp, 397),	/* PRESERVE-WHITESPACE */
		};
		keysort(STACK(base, 3), nargs - 3, 3, keylist, supl_flags, FALSE);
		if(NOT(supl_flags[0]))
		{
			LOAD_FIXNUM(0, STACK(base, 3));
		}
		if(NOT(supl_flags[1]))
		{
			COPY(STACK(base, 0), STACK(base, 6));
			Flength(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 4));
		}
		if(NOT(supl_flags[2]))
		{
			LOAD_NIL(STACK(base, 5));
		}
		BIND_SPECIAL(SYMBOL(Slisp, 378), STACK(base, 5));	/* *PRESERVE-WHITESPACE* */
	}
	LOAD_NIL(STACK(base, 6));
	GEN_HEAPVAR(STACK(base, 6), STACK(base, 7));
	LOAD_FIXNUM(0, STACK(base, 7));
	BIND_SPECIAL(SYMBOL(Slisp, 375), STACK(base, 7));	/* *BQ-LEVEL* */
	COPY(STACK(base, 0), STACK(base, 8));
	COPY(STACK(base, 3), STACK(base, 9));
	COPY(STACK(base, 4), STACK(base, 10));
	Fmake_string_input_stream(STACK(base, 8), 3);
	GEN_HEAPVAR(STACK(base, 8), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
	GEN_HEAPVAR(STACK(base, 9), STACK(base, 10));
	{
		GEN_CLOSURE(array, STACK(base, 10), 8, Z93_g1096, 0);
		COPY(STACK(base, 9), &array[3]);
		COPY(STACK(base, 6), &array[4]);
		COPY(STACK(base, 2), &array[5]);
		COPY(STACK(base, 1), &array[6]);
		COPY(STACK(base, 8), &array[7]);
		LOAD_CLOSURE(array, STACK(base, 10));
	}
	COPY(STACK(base, 10), STACK(base, 10));
	{
		GEN_CLOSURE(array, STACK(base, 11), 4, Z92_g1100, 0);
		COPY(STACK(base, 8), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 11));
	}
	COPY(STACK(base, 11), STACK(base, 11));
	unwind_protect_internal(STACK(base, 10));
	COPY(STACK(base, 10), STACK(base, 0));
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
}

static void Z92_g1100(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	stream_close(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffuncall(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z93_g1096(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 7), STACK(base, 1));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 6), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	internal_read(STACK(base, 1));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
	COPY(STACK(base, 2), &mv_buf[0]);
	mv_count = 2;
	COPY(&mv_buf[0], STACK(base, 2));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		Flist(STACK(base, 1), nargs - 0);
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 7), STACK(base, 2));
		Ffile_position(STACK(base, 2), 1);
		mv_count = 1;
		COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 4));
		{
			static GLOBAL_FUNARG extern_closure = {Fvalues, -1};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 2));
		}
		COPY(STACK(base, 1), STACK(base, 3));
		Fapply(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 1));
	}
	MV_TO_STACK(mv_count, 2);
	{
		int nargs;
		nargs = mv_count;
		mv_count = 1;
		Flist(STACK(base, 1), nargs - 0);
		LOAD_NIL(INDIRECT(GET_FORM(STACK(base, 0)) + 3));
		{
			static GLOBAL_FUNARG extern_closure = {Fvalues, -1};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 2));
		}
		COPY(STACK(base, 1), STACK(base, 3));
		Fapply(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 0));
	}
}
