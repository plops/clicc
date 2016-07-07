#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFpathname_version)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
};

void Fpathname_version(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 3));	/* PATHNAME */
	struct_typep(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
	}
	else
	{
		if(CL_ARRAY_P(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 2));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
			check_array_internal(STACK(base, 2));
			bool_result = CL_TRUEP(STACK(base, 2));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fparse_namestring(STACK(base, 1), 1);
			mv_count = 1;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 2));
			LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 3));	/* STREAM */
			struct_typep(STACK(base, 2));
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(STACK(base, 0), STACK(base, 1));
				file_name(STACK(base, 1), 1);
				Fparse_namestring(STACK(base, 1), 1);
				mv_count = 1;
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFpathname_version[0], STACK(base, 1));	/* etypecase: the value ~a is not a legal value */
				COPY(STACK(base, 0), STACK(base, 2));
				Ferror(STACK(base, 1), 2);
			}
		}
	}
	COPY(STACK(base, 1), STACK(base, 0));
	LOAD_FIXNUM(5, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 2));	/* PATHNAME */
	struct_ref(STACK(base, 0));
}
