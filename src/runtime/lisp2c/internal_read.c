#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kinternal_read)
{
	MAKE_STRING(22, "unexpected End of File"),	/* 0 */
	MAKE_STRING(17, "illegal Character"),	/* 2 */
};

void internal_read(CL_FORM *base)
{
	M99_1:;
	COPY(STACK(base, 0), STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	Fread_char(STACK(base, 4), 3);
	LOAD_NIL(STACK(base, 5));
	if(CL_TRUEP(STACK(base, 4)))
	{
	}
	else
	{
		if(CL_TRUEP(STACK(base, 1)))
		{
			bool_result = CL_TRUEP(STACK(base, 1));
		}
		else
		{
			bool_result = CL_TRUEP(STACK(base, 3));
		}
		if(bool_result)
		{
			LOAD_SMSTR((CL_FORM *)&Kinternal_read[0], STACK(base, 6));	/* unexpected End of File */
			Ferror(STACK(base, 6), 1);
		}
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN110;
	}
	COPY(SYMVAL(Slisp, 382), STACK(base, 6));	/* *READTABLE* */
	LOAD_FIXNUM(0, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 8));	/* READTABLE */
	struct_ref(STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fchar_code(STACK(base, 7));
	vref(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 5));
	if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 390))	/* WHITESPACE */
	{
	}
	else
	{
		if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 393))	/* CONSTITUENT */
		{
			LOAD_T(STACK(base, 6));
		}
		else
		{
			LOAD_NIL(STACK(base, 6));
		}
		if(CL_TRUEP(STACK(base, 6)))
		{
			bool_result = TRUE;
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 391))	/* SINGLE-ESCAPE */
			{
				LOAD_T(STACK(base, 7));
			}
			else
			{
				LOAD_NIL(STACK(base, 7));
			}
			if(CL_TRUEP(STACK(base, 7)))
			{
				bool_result = TRUE;
			}
			else
			{
				bool_result = CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 392);	/* MULTIPLE-ESCAPE */
			}
		}
		if(bool_result)
		{
			LOAD_FIXNUM(0, STACK(base, 6));
			COPY(SYMVAL(Slisp, 376), STACK(base, 7));	/* *TOKEN* */
			Fset_fill_pointer(STACK(base, 6));
			COPY(STACK(base, 0), STACK(base, 6));
			COPY(STACK(base, 4), STACK(base, 7));
			read_token(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 4));
			goto RETURN111;
		}
		else
		{
			if(CL_TRUEP(STACK(base, 5)))
			{
				COPY(STACK(base, 5), STACK(base, 6));
				Fcar(STACK(base, 6));
				COPY(STACK(base, 0), STACK(base, 7));
				COPY(STACK(base, 4), STACK(base, 8));
				Ffuncall(STACK(base, 6), 3);
				MV_TO_STACK(mv_count, 7);
				{
					int nargs;
					nargs = mv_count;
					mv_count = 1;
					MEM_UP_MOVE(STACK(base, 6 + nargs), STACK(base, 6), 1);
					LOAD_NIL(STACK(base, 6));
					nargs = nargs + 1;
					if(nargs > 2)
					{
						goto ALL_OPT_SUPPLIED;
					}
					switch(nargs)
					{
						case 2:
						ALL_OPT_SUPPLIED:;
						LOAD_T(STACK(base, 6));
					}
					switch(nargs)
					{
						case 1:
						LOAD_NIL(STACK(base, 7));
						case 2:
						break;
						default:
						Labort(TOO_MANY_ARGS);
					}
					if(CL_TRUEP(STACK(base, 6)))
					{
						COPY(STACK(base, 7), STACK(base, 4));
						goto RETURN111;
					}
				}
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&Kinternal_read[2], STACK(base, 6));	/* illegal Character */
				Ferror(STACK(base, 6), 1);
			}
		}
	}
	goto M99_1;
	RETURN111:;
	if(CL_TRUEP(STACK(base, 3)))
	{
		bool_result = FALSE;
	}
	else
	{
		bool_result = NOT(CL_TRUEP(SYMVAL(Slisp, 378)));	/* *PRESERVE-WHITESPACE* */
	}
	if(bool_result)
	{
		COPY(STACK(base, 0), STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
		LOAD_NIL(STACK(base, 7));
		Fread_char(STACK(base, 5), 3);
		if(NOT(CL_TRUEP(STACK(base, 5))))
		{
			LOAD_T(STACK(base, 6));
		}
		else
		{
			LOAD_NIL(STACK(base, 6));
		}
		if(CL_TRUEP(STACK(base, 6)))
		{
		}
		else
		{
			COPY(SYMVAL(Slisp, 382), STACK(base, 7));	/* *READTABLE* */
			LOAD_FIXNUM(0, STACK(base, 8));
			LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 9));	/* READTABLE */
			struct_ref(STACK(base, 7));
			COPY(STACK(base, 5), STACK(base, 8));
			Fchar_code(STACK(base, 8));
			vref(STACK(base, 7));
			if(CL_SYMBOLP(STACK(base, 7)) && GET_SYMBOL(STACK(base, 7)) == SYMBOL(Slisp, 390))	/* WHITESPACE */
			{
				LOAD_T(STACK(base, 7));
			}
			else
			{
				LOAD_NIL(STACK(base, 7));
			}
			if(CL_TRUEP(STACK(base, 7)))
			{
			}
			else
			{
				COPY(STACK(base, 5), STACK(base, 8));
				COPY(STACK(base, 0), STACK(base, 9));
				Funread_char(STACK(base, 8), 2);
				mv_count = 1;
			}
		}
	}
	COPY(STACK(base, 4), STACK(base, 0));
	RETURN110:;
}
