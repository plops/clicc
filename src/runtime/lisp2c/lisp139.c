#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void unix_file_kind(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		unix_link_mode(STACK(base, 2));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		unix_file_mode(STACK(base, 2));
	}
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 2), STACK(base, 3));
		LOAD_FIXNUM(61440, STACK(base, 4));
		Flogand(STACK(base, 3), 2);
		if(CL_FIXNUMP(STACK(base, 3)) && GET_FIXNUM(STACK(base, 3)) == 16384)
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 184), STACK(base, 0));	/* DIRECTORY */
		}
		else
		{
			if(CL_FIXNUMP(STACK(base, 3)) && GET_FIXNUM(STACK(base, 3)) == 32768)
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 234), STACK(base, 0));	/* FILE */
			}
			else
			{
				if(CL_FIXNUMP(STACK(base, 3)) && GET_FIXNUM(STACK(base, 3)) == 40960)
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 235), STACK(base, 0));	/* LINK */
				}
				else
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 236), STACK(base, 0));	/* SPECIAL */
				}
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
