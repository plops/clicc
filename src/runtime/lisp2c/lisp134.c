#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kquick_integer_to_string)
{
	MAKE_STRING(1, "0"),	/* 0 */
	MAKE_STRING(1, "1"),	/* 2 */
	MAKE_STRING(1, "-"),	/* 4 */
	MAKE_STRING(10, "0123456789"),	/* 6 */
};

void quick_integer_to_string(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	Fzerop(STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
		LOAD_SMSTR((CL_FORM *)&Kquick_integer_to_string[0], STACK(base, 0));	/* 0 */
	}
	else
	{
		if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 1)
		{
			LOAD_SMSTR((CL_FORM *)&Kquick_integer_to_string[2], STACK(base, 0));	/* 1 */
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fminusp(STACK(base, 1));
			if(CL_TRUEP(STACK(base, 1)))
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 1));	/* SIMPLE-STRING */
				LOAD_SMSTR((CL_FORM *)&Kquick_integer_to_string[4], STACK(base, 2));	/* - */
				COPY(STACK(base, 0), STACK(base, 3));
				Fminus(STACK(base, 3), 1);
				quick_integer_to_string(STACK(base, 3));
				if(CL_SMAR_P(STACK(base, 3)))
				{
					COPY(STACK(base, 3), STACK(base, 4));
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
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KClisp[8], STACK(base, 4));	/* type error: ~S is not of type ~S */
					COPY(STACK(base, 3), STACK(base, 5));
					LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 6));	/* SIMPLE-STRING */
					Ferror(STACK(base, 4), 3);
				}
				Fconcatenate(STACK(base, 1), 3);
				COPY(STACK(base, 1), STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 1));
				Finteger_length(STACK(base, 1));
				LOAD_FIXNUM(3, STACK(base, 2));
				truncate_internal(STACK(base, 1));
				F1plus(STACK(base, 1));
				COPY(STACK(base, 1), STACK(base, 2));
				Fmake_string(STACK(base, 2), 1);
				COPY(STACK(base, 1), STACK(base, 3));
				F1minus(STACK(base, 3));
				COPY(STACK(base, 0), STACK(base, 4));
				LOAD_FIXNUM(0, STACK(base, 5));
				M35_1:;
				COPY(STACK(base, 4), STACK(base, 6));
				Fzerop(STACK(base, 6));
				if(CL_TRUEP(STACK(base, 6)))
				{
					COPY(STACK(base, 3), STACK(base, 6));
					F1plus(STACK(base, 6));
					COPY(STACK(base, 6), STACK(base, 3));
					COPY(STACK(base, 2), STACK(base, 6));
					COPY(STACK(base, 2), STACK(base, 7));
					LOAD_SYMBOL(SYMBOL(Slisp, 211), STACK(base, 8));	/* START2 */
					COPY(STACK(base, 3), STACK(base, 9));
					LOAD_SYMBOL(SYMBOL(Slisp, 212), STACK(base, 10));	/* END2 */
					COPY(STACK(base, 1), STACK(base, 11));
					Freplace(STACK(base, 6), 6);
					COPY(STACK(base, 2), STACK(base, 0));
					COPY(STACK(base, 3), STACK(base, 2));
					Fminus(STACK(base, 1), 2);
					shrink_vector(STACK(base, 0));
					goto RETURN36;
				}
				COPY(STACK(base, 4), STACK(base, 6));
				LOAD_FIXNUM(10, STACK(base, 7));
				truncate_internal(STACK(base, 6));
				COPY(&mv_buf[0], STACK(base, 7));
				{
					int nargs;
					nargs = 2;
					mv_count = 1;
					switch(nargs)
					{
						case 0:
						LOAD_NIL(STACK(base, 6));
						case 1:
						LOAD_NIL(STACK(base, 7));
						nargs = 2;
					}
					Flist(STACK(base, 8), nargs - 2);
					COPY(STACK(base, 6), STACK(base, 4));
					COPY(STACK(base, 7), STACK(base, 5));
				}
				LOAD_SMSTR((CL_FORM *)&Kquick_integer_to_string[6], STACK(base, 6));	/* 0123456789 */
				COPY(STACK(base, 5), STACK(base, 7));
				Fschar(STACK(base, 6));
				COPY(STACK(base, 6), STACK(base, 7));
				COPY(STACK(base, 2), STACK(base, 8));
				COPY(STACK(base, 3), STACK(base, 9));
				Fset_schar(STACK(base, 7));
				COPY(STACK(base, 3), STACK(base, 6));
				F1minus(STACK(base, 6));
				COPY(STACK(base, 6), STACK(base, 3));
				goto M35_1;
				RETURN36:;
			}
		}
	}
}
