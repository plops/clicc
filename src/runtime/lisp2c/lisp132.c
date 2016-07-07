#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KPenumerate_directories)
{
	MAKE_STRING(1, "/"),	/* 0 */
	MAKE_STRING(3, "../"),	/* 2 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 4 */
};

void Penumerate_directories(CL_FORM *base)
{
	M34_1:;
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 5));
		Fcar(STACK(base, 5));
		if(CL_SMAR_P(STACK(base, 5)))
		{
			COPY(STACK(base, 5), STACK(base, 6));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 7));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 8));	/* * */
			check_array_internal(STACK(base, 6));
			bool_result = CL_TRUEP(STACK(base, 6));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 6));	/* STRING */
			COPY(STACK(base, 0), STACK(base, 7));
			COPY(STACK(base, 5), STACK(base, 8));
			LOAD_SMSTR((CL_FORM *)&KPenumerate_directories[0], STACK(base, 9));	/* / */
			Fconcatenate(STACK(base, 6), 4);
			COPY(STACK(base, 1), STACK(base, 7));
			Fcdr(STACK(base, 7));
			COPY(STACK(base, 6), STACK(base, 0));
			COPY(STACK(base, 7), STACK(base, 1));
			goto M34_1;
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 205))	/* UP */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 6));	/* STRING */
				COPY(STACK(base, 0), STACK(base, 7));
				LOAD_SMSTR((CL_FORM *)&KPenumerate_directories[2], STACK(base, 8));	/* ../ */
				Fconcatenate(STACK(base, 6), 3);
				COPY(STACK(base, 1), STACK(base, 7));
				Fcdr(STACK(base, 7));
				COPY(STACK(base, 6), STACK(base, 0));
				COPY(STACK(base, 7), STACK(base, 1));
				goto M34_1;
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KPenumerate_directories[4], STACK(base, 0));	/* etypecase: the value ~a is not a legal value */
				COPY(STACK(base, 5), STACK(base, 1));
				Ferror(STACK(base, 0), 2);
			}
		}
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 1));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
		COPY(STACK(base, 4), STACK(base, 3));
		Penumerate_files(STACK(base, 0));
	}
	goto RETURN35;
	RETURN35:;
}
