#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cradix_reader = {radix_reader, 3};

CL_INIT2(Kradix_reader)
{
	MAKE_STRING(42, "#~A (base ~D) value is not a rational: ~S."),	/* 0 */
	MAKE_STRING(25, "Illegal radix for #R: ~D."),	/* 2 */
	MAKE_STRING(20, "Radix missing in #R."),	/* 4 */
};

void radix_reader(CL_FORM *base)
{
	if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
	{
		COPY(STACK(base, 0), STACK(base, 3));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
		LOAD_NIL(STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		Fread_char(STACK(base, 3), 4);
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 5));
		read_token(STACK(base, 4));
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_FIXNUM(2, STACK(base, 3));
			COPY(STACK(base, 2), STACK(base, 4));
			LOAD_FIXNUM(36, STACK(base, 5));
			Fle(STACK(base, 3), 3);
			if(CL_TRUEP(STACK(base, 3)))
			{
				COPY(STACK(base, 2), STACK(base, 3));
				BIND_SPECIAL(SYMBOL(Slisp, 345), STACK(base, 3));	/* *READ-BASE* */
				COPY(STACK(base, 0), STACK(base, 3));
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
				LOAD_NIL(STACK(base, 5));
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
				Fread(STACK(base, 3), 4);
				RESTORE_SPECIAL;
				if(CL_FIXNUMP(STACK(base, 3)))
				{
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kradix_reader[0], STACK(base, 4));	/* #~A (base ~D) value is not a rational: ~S. */
					COPY(STACK(base, 1), STACK(base, 5));
					COPY(STACK(base, 2), STACK(base, 6));
					COPY(STACK(base, 3), STACK(base, 7));
					Ferror(STACK(base, 4), 4);
				}
				COPY(STACK(base, 3), STACK(base, 0));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&Kradix_reader[2], STACK(base, 0));	/* Illegal radix for #R: ~D. */
				COPY(STACK(base, 2), STACK(base, 1));
				Ferror(STACK(base, 0), 2);
			}
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kradix_reader[4], STACK(base, 0));	/* Radix missing in #R. */
			Ferror(STACK(base, 0), 1);
		}
	}
}
