#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kunix_maybe_prepend_current_directory)
{
	MAKE_STRING(1, "/"),	/* 0 */
};

void unix_maybe_prepend_current_directory(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Flength(STACK(base, 1));
	Fplusp(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
		LOAD_FIXNUM(0, STACK(base, 2));
		Fschar(STACK(base, 1));
		LOAD_CHAR('/', STACK(base, 2));
		FcharE(STACK(base, 1), 2);
		bool_result = CL_TRUEP(STACK(base, 1));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
	}
	else
	{
		unix_current_directory(STACK(base, 1));
		COPY(&mv_buf[0], STACK(base, 2));
		{
			int nargs;
			nargs = 2;
			mv_count = 1;
			switch(nargs)
			{
				case 0:
				LOAD_NIL(STACK(base, 1));
				case 1:
				LOAD_NIL(STACK(base, 2));
				nargs = 2;
			}
			Flist(STACK(base, 3), nargs - 2);
			if(CL_TRUEP(STACK(base, 1)))
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 4));	/* SIMPLE-STRING */
				COPY(STACK(base, 2), STACK(base, 5));
				LOAD_SMSTR((CL_FORM *)&Kunix_maybe_prepend_current_directory[0], STACK(base, 6));	/* / */
				COPY(STACK(base, 0), STACK(base, 7));
				Fconcatenate(STACK(base, 4), 4);
				COPY(STACK(base, 4), STACK(base, 0));
			}
			else
			{
			}
		}
	}
}
