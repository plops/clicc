#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void remf_internal(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
		LOAD_NIL(STACK(base, 2));
		LOAD_NIL(STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 0));
		COPY(STACK(base, 3), &mv_buf[0]);
		mv_count = 2;
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fcar(STACK(base, 2));
		if(EQ(STACK(base, 2), STACK(base, 1)))
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fcdr(STACK(base, 2));
			Fcdr(STACK(base, 2));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
			COPY(STACK(base, 2), STACK(base, 0));
			COPY(STACK(base, 3), &mv_buf[0]);
			mv_count = 2;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			Fcdr(STACK(base, 3));
			Fcdr(STACK(base, 3));
			M240_1:;
			if(CL_ATOMP(STACK(base, 3)))
			{
				LOAD_NIL(STACK(base, 4));
				LOAD_NIL(STACK(base, 5));
				COPY(STACK(base, 4), STACK(base, 0));
				COPY(STACK(base, 5), &mv_buf[0]);
				mv_count = 2;
				goto RETURN281;
			}
			COPY(STACK(base, 3), STACK(base, 4));
			Fcar(STACK(base, 4));
			if(EQ(STACK(base, 4), STACK(base, 1)))
			{
				COPY(STACK(base, 2), STACK(base, 4));
				Fcdr(STACK(base, 4));
				COPY(STACK(base, 3), STACK(base, 5));
				Fcdr(STACK(base, 5));
				Fcdr(STACK(base, 5));
				if(CL_CONSP(STACK(base, 4)))
				{
					COPY(STACK(base, 5), CDR(GET_FORM(STACK(base, 4))));
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KClisp[2], STACK(base, 6));	/* ~a is not a cons */
					COPY(STACK(base, 4), STACK(base, 7));
					Ferror(STACK(base, 6), 2);
				}
				COPY(STACK(base, 0), STACK(base, 4));
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
				COPY(STACK(base, 4), STACK(base, 0));
				COPY(STACK(base, 5), &mv_buf[0]);
				mv_count = 2;
				goto RETURN280;
			}
			COPY(STACK(base, 2), STACK(base, 4));
			Fcdr(STACK(base, 4));
			Fcdr(STACK(base, 4));
			COPY(STACK(base, 3), STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 6));
			Fcdr(STACK(base, 6));
			Fcdr(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 3));
			COPY(STACK(base, 4), STACK(base, 2));
			goto M240_1;
			RETURN281:;
		}
	}
	RETURN280:;
}
