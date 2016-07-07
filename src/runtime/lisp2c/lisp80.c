#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFfile_position)
{
	MAKE_STRING(21, "illegal file-position"),	/* 0 */
	MAKE_STRING(25, "illegal file-position: ~S"),	/* 2 */
};

void Ffile_position(CL_FORM *base, int nargs)
{
	MEM_UP_MOVE(STACK(base, 0 + nargs), STACK(base, 1), 1);
	LOAD_NIL(STACK(base, 1));
	nargs = nargs + 1;
	if(nargs > 3)
	{
		goto ALL_OPT_SUPPLIED;
	}
	switch(nargs)
	{
		case 3:
		ALL_OPT_SUPPLIED:;
		LOAD_T(STACK(base, 1));
	}
	switch(nargs)
	{
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		if(CL_FIXNUMP(STACK(base, 2)))
		{
			LOAD_FIXNUM(0, STACK(base, 3));
			COPY(STACK(base, 2), STACK(base, 4));
			COPY(STACK(base, 0), STACK(base, 5));
			stream_length(STACK(base, 5));
			Ffuncall(STACK(base, 5), 1);
			Fle(STACK(base, 3), 3);
			if(CL_TRUEP(STACK(base, 3)))
			{
				LOAD_NIL(STACK(base, 2));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFfile_position[0], STACK(base, 2));	/* illegal file-position */
				Ferror(STACK(base, 2), 1);
			}
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 161))	/* START */
			{
				LOAD_FIXNUM(0, STACK(base, 2));
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 162))	/* END */
				{
					COPY(STACK(base, 0), STACK(base, 3));
					COPY(STACK(base, 3), STACK(base, 2));
					stream_length(STACK(base, 2));
					Ffuncall(STACK(base, 2), 1);
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFfile_position[2], STACK(base, 3));	/* illegal file-position: ~S */
					COPY(STACK(base, 2), STACK(base, 4));
					Ferror(STACK(base, 3), 2);
				}
			}
		}
		COPY(STACK(base, 0), STACK(base, 3));
		stream_seek(STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		Ffuncall(STACK(base, 3), 2);
		COPY(STACK(base, 3), STACK(base, 0));
	}
	else
	{
		stream_tell(STACK(base, 0));
		Ffuncall(STACK(base, 0), 1);
	}
}
