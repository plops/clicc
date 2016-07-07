#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Ccomment_block_reader = {comment_block_reader, 3};

void comment_block_reader(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[4], STACK(base, 3));	/* extra argument for #~S */
		LOAD_CHAR('|', STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	LOAD_NIL(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	Fread_char(STACK(base, 3), 4);
	LOAD_FIXNUM(0, STACK(base, 4));
	M98_1:;
	if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '|')
	{
		COPY(STACK(base, 0), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		LOAD_NIL(STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
		Fread_char(STACK(base, 5), 4);
		COPY(STACK(base, 5), STACK(base, 3));
		if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '#')
		{
			if(CL_FIXNUMP(STACK(base, 4)) && GET_FIXNUM(STACK(base, 4)) == 0)
			{
				LOAD_NIL(STACK(base, 0));
				mv_count = 0;
				goto RETURN109;
			}
			F1minus(STACK(base, 4));
			COPY(STACK(base, 0), STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
			LOAD_NIL(STACK(base, 7));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
			Fread_char(STACK(base, 5), 4);
			COPY(STACK(base, 5), STACK(base, 3));
		}
	}
	else
	{
		if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '#')
		{
			COPY(STACK(base, 0), STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
			LOAD_NIL(STACK(base, 7));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
			Fread_char(STACK(base, 5), 4);
			COPY(STACK(base, 5), STACK(base, 3));
			if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '|')
			{
				F1plus(STACK(base, 4));
				COPY(STACK(base, 0), STACK(base, 5));
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
				LOAD_NIL(STACK(base, 7));
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
				Fread_char(STACK(base, 5), 4);
				COPY(STACK(base, 5), STACK(base, 3));
			}
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
			LOAD_NIL(STACK(base, 7));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
			Fread_char(STACK(base, 5), 4);
			COPY(STACK(base, 5), STACK(base, 3));
		}
	}
	goto M98_1;
	RETURN109:;
}
