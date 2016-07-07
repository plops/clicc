#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFenough_namestring)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 2 */
	MAKE_STRING(65, "Cannot determine the namestring for pathnames with no host:~%  ~S"),	/* 4 */
};

void Fenough_namestring(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 163), STACK(base, 1));	/* *DEFAULT-PATHNAME-DEFAULTS* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 4));	/* PATHNAME */
	struct_typep(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
	}
	else
	{
		if(CL_ARRAY_P(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 4));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 5));	/* * */
			check_array_internal(STACK(base, 3));
			bool_result = CL_TRUEP(STACK(base, 3));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fparse_namestring(STACK(base, 2), 1);
			mv_count = 1;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
			struct_typep(STACK(base, 3));
			if(CL_TRUEP(STACK(base, 3)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				file_name(STACK(base, 2), 1);
				Fparse_namestring(STACK(base, 2), 1);
				mv_count = 1;
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFenough_namestring[0], STACK(base, 2));	/* etypecase: the value ~a is not a legal value */
				COPY(STACK(base, 0), STACK(base, 3));
				Ferror(STACK(base, 2), 2);
			}
		}
	}
	COPY(STACK(base, 2), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
	struct_ref(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
		struct_typep(STACK(base, 5));
		if(CL_TRUEP(STACK(base, 5)))
		{
			COPY(STACK(base, 1), STACK(base, 4));
		}
		else
		{
			if(CL_ARRAY_P(STACK(base, 1)))
			{
				COPY(STACK(base, 1), STACK(base, 5));
				LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 6));	/* STANDARD-CHAR */
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 7));	/* * */
				check_array_internal(STACK(base, 5));
				bool_result = CL_TRUEP(STACK(base, 5));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 1), STACK(base, 4));
				Fparse_namestring(STACK(base, 4), 1);
				mv_count = 1;
			}
			else
			{
				COPY(STACK(base, 1), STACK(base, 5));
				LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 6));	/* STREAM */
				struct_typep(STACK(base, 5));
				if(CL_TRUEP(STACK(base, 5)))
				{
					COPY(STACK(base, 1), STACK(base, 4));
					file_name(STACK(base, 4), 1);
					Fparse_namestring(STACK(base, 4), 1);
					mv_count = 1;
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFenough_namestring[2], STACK(base, 4));	/* etypecase: the value ~a is not a legal value */
					COPY(STACK(base, 1), STACK(base, 5));
					Ferror(STACK(base, 4), 2);
				}
			}
		}
		COPY(STACK(base, 3), STACK(base, 5));
		host_unparse_enough(STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		COPY(STACK(base, 4), STACK(base, 7));
		Ffuncall(STACK(base, 5), 3);
		COPY(STACK(base, 5), STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFenough_namestring[4], STACK(base, 0));	/* Cannot determine the namestring for pathnames with no host:~%  ~S */
		COPY(STACK(base, 2), STACK(base, 1));
		Ferror(STACK(base, 0), 2);
	}
}
