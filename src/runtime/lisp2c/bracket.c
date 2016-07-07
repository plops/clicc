#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void bracket(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
		COPY(SYMVAL(Slisp, 367), STACK(base, 1));	/* *BQ-LIST* */
		COPY(STACK(base, 0), STACK(base, 2));
		bq_process(STACK(base, 2));
		Flist(STACK(base, 1), 2);
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(EQ(STACK(base, 1), SYMVAL(Slisp, 364)))	/* *COMMA* */
		{
			COPY(SYMVAL(Slisp, 367), STACK(base, 1));	/* *BQ-LIST* */
			COPY(STACK(base, 0), STACK(base, 2));
			Fcdr(STACK(base, 2));
			Flist(STACK(base, 1), 2);
			COPY(STACK(base, 1), STACK(base, 0));
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
			if(EQ(STACK(base, 1), SYMVAL(Slisp, 365)))	/* *COMMA-ATSIGN* */
			{
				Fcdr(STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 1));
				Fcar(STACK(base, 1));
				if(EQ(STACK(base, 1), SYMVAL(Slisp, 366)))	/* *COMMA-DOT* */
				{
					COPY(SYMVAL(Slisp, 371), STACK(base, 1));	/* *BQ-CLOBBERABLE* */
					COPY(STACK(base, 0), STACK(base, 2));
					Fcdr(STACK(base, 2));
					Flist(STACK(base, 1), 2);
					COPY(STACK(base, 1), STACK(base, 0));
				}
				else
				{
					COPY(SYMVAL(Slisp, 367), STACK(base, 1));	/* *BQ-LIST* */
					COPY(STACK(base, 0), STACK(base, 2));
					bq_process(STACK(base, 2));
					Flist(STACK(base, 1), 2);
					COPY(STACK(base, 1), STACK(base, 0));
				}
			}
		}
	}
}
