#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cchar_reader = {char_reader, 3};

CL_INIT2(Kchar_reader)
{
	MAKE_STRING(25, "illegal character name ~s"),	/* 0 */
	MAKE_STRING(29, "font ~s of ~s will be ignored"),	/* 2 */
};

void char_reader(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	LOAD_NIL(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	Fread_char(STACK(base, 3), 4);
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	LOAD_NIL(STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
	Fpeek_char(STACK(base, 4), 5);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(SYMVAL(Slisp, 382), STACK(base, 5));	/* *READTABLE* */
		LOAD_FIXNUM(0, STACK(base, 6));
		LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 7));	/* READTABLE */
		struct_ref(STACK(base, 5));
		COPY(STACK(base, 4), STACK(base, 6));
		Fchar_code(STACK(base, 6));
		vref(STACK(base, 5));
		bool_result = CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 393);	/* CONSTITUENT */
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_FIXNUM(0, STACK(base, 5));
		COPY(SYMVAL(Slisp, 376), STACK(base, 6));	/* *TOKEN* */
		Fset_fill_pointer(STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
		BIND_SPECIAL(SYMBOL(Slisp, 346), STACK(base, 5));	/* *READ-SUPPRESS* */
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 3), STACK(base, 7));
		read_token(STACK(base, 6));
		RESTORE_SPECIAL;
		if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
		{
		}
		else
		{
			COPY(SYMVAL(Slisp, 376), STACK(base, 5));	/* *TOKEN* */
			Fname_char(STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 3));
			if(CL_TRUEP(STACK(base, 3)))
			{
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&Kchar_reader[0], STACK(base, 5));	/* illegal character name ~s */
				COPY(SYMVAL(Slisp, 376), STACK(base, 6));	/* *TOKEN* */
				Ferror(STACK(base, 5), 2);
			}
		}
	}
	if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		if(CL_TRUEP(STACK(base, 2)))
		{
			COPY(STACK(base, 2), STACK(base, 5));
			Fplusp(STACK(base, 5));
			bool_result = CL_TRUEP(STACK(base, 5));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			LOAD_SMSTR((CL_FORM *)&Kchar_reader[2], STACK(base, 5));	/* font ~s of ~s will be ignored */
			COPY(STACK(base, 2), STACK(base, 6));
			COPY(STACK(base, 3), STACK(base, 7));
			Fwarn(STACK(base, 5), 3);
			mv_count = 1;
		}
		COPY(STACK(base, 3), STACK(base, 0));
	}
}
