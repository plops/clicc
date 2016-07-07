#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cright_parenthesis_reader = {right_parenthesis_reader, 2};

CL_INIT2(Kright_parenthesis_reader)
{
	MAKE_STRING(24, "Ignoring an unmatched ~a"),	/* 0 */
};

void right_parenthesis_reader(CL_FORM *base)
{
	if(EQ(STACK(base, 1), SYMVAL(Slisp, 380)) || CL_FLOATP(STACK(base, 1)) && GET_FLOAT(STACK(base, 1)) == GET_FLOAT(SYMVAL(Slisp, 380)))	/* *PARENTHESIS-OPEN* */
	{
		LOAD_NIL(SYMVAL(Slisp, 380));	/* *PARENTHESIS-OPEN* */
		COPY(SYMVAL(Slisp, 380), STACK(base, 0));	/* *PARENTHESIS-OPEN* */
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&Kright_parenthesis_reader[0], STACK(base, 2));	/* Ignoring an unmatched ~a */
		COPY(STACK(base, 1), STACK(base, 3));
		Fwarn(STACK(base, 2), 2);
		mv_count = 1;
		LOAD_NIL(STACK(base, 0));
		mv_count = 0;
	}
}
