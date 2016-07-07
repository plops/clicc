#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmerge_pathnames)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
};

void Fmerge_pathnames(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 163), STACK(base, 1));	/* *DEFAULT-PATHNAME-DEFAULTS* */
		case 2:
		LOAD_SYMBOL(SYMBOL(Slisp, 200), STACK(base, 2));	/* NEWEST */
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
	struct_typep(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 3));
	}
	else
	{
		if(CL_ARRAY_P(STACK(base, 1)))
		{
			COPY(STACK(base, 1), STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 5));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
			check_array_internal(STACK(base, 4));
			bool_result = CL_TRUEP(STACK(base, 4));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 1), STACK(base, 3));
			Fparse_namestring(STACK(base, 3), 1);
			mv_count = 1;
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 5));	/* STREAM */
			struct_typep(STACK(base, 4));
			if(CL_TRUEP(STACK(base, 4)))
			{
				COPY(STACK(base, 1), STACK(base, 3));
				file_name(STACK(base, 3), 1);
				Fparse_namestring(STACK(base, 3), 1);
				mv_count = 1;
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFmerge_pathnames[0], STACK(base, 3));	/* etypecase: the value ~a is not a legal value */
				COPY(STACK(base, 1), STACK(base, 4));
				Ferror(STACK(base, 3), 2);
			}
		}
	}
	COPY(STACK(base, 3), STACK(base, 4));
	BIND_SPECIAL(SYMBOL(Slisp, 163), STACK(base, 4));	/* *DEFAULT-PATHNAME-DEFAULTS* */
	COPY(STACK(base, 0), STACK(base, 4));
	Fpathname(STACK(base, 4));
	RESTORE_SPECIAL;
	COPY(STACK(base, 3), STACK(base, 5));
	LOAD_FIXNUM(0, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
	struct_ref(STACK(base, 5));
	COPY(STACK(base, 4), STACK(base, 6));
	LOAD_FIXNUM(0, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
	struct_ref(STACK(base, 6));
	if(CL_TRUEP(STACK(base, 5)))
	{
		if(CL_TRUEP(STACK(base, 6)))
		{
			COPY(STACK(base, 5), STACK(base, 7));
			LOAD_FIXNUM(6, STACK(base, 8));
			LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 9));	/* HOST */
			struct_ref(STACK(base, 7));
			COPY(STACK(base, 6), STACK(base, 8));
			LOAD_FIXNUM(6, STACK(base, 9));
			LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 10));	/* HOST */
			struct_ref(STACK(base, 8));
			if(NOT(EQ(STACK(base, 7), STACK(base, 8))))
			{
				LOAD_T(STACK(base, 7));
			}
			else
			{
				LOAD_NIL(STACK(base, 7));
			}
		}
		else
		{
			LOAD_NIL(STACK(base, 7));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 7));
	}
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 6), STACK(base, 8));
	}
	else
	{
		COPY(STACK(base, 5), STACK(base, 8));
	}
	COPY(STACK(base, 4), STACK(base, 9));
	LOAD_FIXNUM(1, STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 11));	/* PATHNAME */
	struct_ref(STACK(base, 9));
	if(CL_TRUEP(STACK(base, 9)))
	{
	}
	else
	{
		COPY(STACK(base, 3), STACK(base, 9));
		LOAD_FIXNUM(1, STACK(base, 10));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 11));	/* PATHNAME */
		struct_ref(STACK(base, 9));
		COPY(STACK(base, 7), STACK(base, 10));
		maybe_diddle_case(STACK(base, 9));
		mv_count = 1;
	}
	COPY(STACK(base, 4), STACK(base, 10));
	LOAD_FIXNUM(2, STACK(base, 11));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 12));	/* PATHNAME */
	struct_ref(STACK(base, 10));
	COPY(STACK(base, 3), STACK(base, 11));
	LOAD_FIXNUM(2, STACK(base, 12));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 13));	/* PATHNAME */
	struct_ref(STACK(base, 11));
	COPY(STACK(base, 7), STACK(base, 12));
	merge_directories(STACK(base, 10));
	COPY(STACK(base, 4), STACK(base, 11));
	LOAD_FIXNUM(3, STACK(base, 12));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 13));	/* PATHNAME */
	struct_ref(STACK(base, 11));
	if(CL_TRUEP(STACK(base, 11)))
	{
	}
	else
	{
		COPY(STACK(base, 3), STACK(base, 11));
		LOAD_FIXNUM(3, STACK(base, 12));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 13));	/* PATHNAME */
		struct_ref(STACK(base, 11));
		COPY(STACK(base, 7), STACK(base, 12));
		maybe_diddle_case(STACK(base, 11));
		mv_count = 1;
	}
	COPY(STACK(base, 4), STACK(base, 12));
	LOAD_FIXNUM(4, STACK(base, 13));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 14));	/* PATHNAME */
	struct_ref(STACK(base, 12));
	if(CL_TRUEP(STACK(base, 12)))
	{
	}
	else
	{
		COPY(STACK(base, 3), STACK(base, 12));
		LOAD_FIXNUM(4, STACK(base, 13));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 14));	/* PATHNAME */
		struct_ref(STACK(base, 12));
		COPY(STACK(base, 7), STACK(base, 13));
		maybe_diddle_case(STACK(base, 12));
		mv_count = 1;
	}
	COPY(STACK(base, 4), STACK(base, 13));
	LOAD_FIXNUM(5, STACK(base, 14));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 15));	/* PATHNAME */
	struct_ref(STACK(base, 13));
	if(CL_TRUEP(STACK(base, 13)))
	{
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 13));
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 0));	/* PATHNAME */
	COPY(STACK(base, 8), STACK(base, 1));
	COPY(STACK(base, 9), STACK(base, 2));
	COPY(STACK(base, 10), STACK(base, 3));
	COPY(STACK(base, 11), STACK(base, 4));
	COPY(STACK(base, 12), STACK(base, 5));
	COPY(STACK(base, 13), STACK(base, 6));
	make_struct(STACK(base, 0), 7);
}
