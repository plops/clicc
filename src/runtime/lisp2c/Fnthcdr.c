/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void Fnthcdr(CL_FORM *base)
{
	M1_1:;
	COPY(ARG(0), ARG(2));
	Fzerop(ARG(2));
	if(CL_TRUEP(ARG(2)))
	{
		COPY(ARG(1), ARG(0));
		goto RETURN1;
	}
	else
	{
		if(CL_TRUEP(ARG(1)))
		{
			COPY(ARG(0), ARG(2));
			F1minus(ARG(2));
			COPY(ARG(2), ARG(0));
			if(CL_CONSP(ARG(1)))
			{
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KClisp[239], ARG(2));	/* ~a is not a list */
				COPY(ARG(1), ARG(3));
				Ferror(ARG(2), 2);
			}
			COPY(ARG(1), ARG(2));
			COPY(ARG(2), ARG(3));
			if(CL_CONSP(ARG(3)))
			{
				COPY(GET_CDR(ARG(3)), ARG(1));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KClisp[241], ARG(1));	/* ~a is not a list */
				COPY(ARG(3), ARG(2));
				Ferror(ARG(1), 2);
			}
		}
		else
		{
			LOAD_NIL(ARG(0));
			goto RETURN1;
		}
	}
	goto M1_1;
	RETURN1:;
}
