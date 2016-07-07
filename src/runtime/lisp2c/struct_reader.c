#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cstruct_reader = {struct_reader, 3};

CL_INIT2(Kstruct_reader)
{
	MAKE_STRING(30, "illegal value (~s) followed #S"),	/* 0 */
	MAKE_STRING(21, "~a is not a structure"),	/* 2 */
	MAKE_STRING(19, "~s should be a list"),	/* 4 */
};

void struct_reader(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[4], STACK(base, 3));	/* extra argument for #~S */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	LOAD_NIL(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	Fread(STACK(base, 3), 4);
	if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		if(CL_ATOMP(STACK(base, 3)))
		{
			LOAD_SMSTR((CL_FORM *)&Kstruct_reader[0], STACK(base, 0));	/* illegal value (~s) followed #S */
			COPY(STACK(base, 3), STACK(base, 1));
			Ferror(STACK(base, 0), 2);
		}
		else
		{
			COPY(STACK(base, 3), STACK(base, 4));
			Fcar(STACK(base, 4));
			struct_constructor(STACK(base, 4));
			if(CL_TRUEP(STACK(base, 4)))
			{
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&Kstruct_reader[2], STACK(base, 5));	/* ~a is not a structure */
				COPY(STACK(base, 3), STACK(base, 6));
				Fcar(STACK(base, 6));
				Ferror(STACK(base, 5), 2);
			}
			COPY(STACK(base, 3), STACK(base, 5));
			Fcdr(STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 3));
			LOAD_NIL(STACK(base, 5));
			M95_1:;
			COPY(STACK(base, 3), STACK(base, 6));
			Fendp(STACK(base, 6));
			if(CL_TRUEP(STACK(base, 6)))
			{
				COPY(STACK(base, 4), STACK(base, 0));
				COPY(STACK(base, 5), STACK(base, 1));
				Fnreverse(STACK(base, 1));
				Fapply(STACK(base, 0), 2);
				goto RETURN105;
			}
			COPY(STACK(base, 3), STACK(base, 6));
			Fcar(STACK(base, 6));
			Fstring(STACK(base, 6));
			COPY(SYMVAL(Slisp, 303), STACK(base, 7));	/* *KEYWORD-PACKAGE* */
			Fintern(STACK(base, 6), 2);
			mv_count = 1;
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 6), CAR(lptr));
				COPY(STACK(base, 5), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 5));
			}
			COPY(STACK(base, 3), STACK(base, 6));
			Fcdr(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 3));
			if(CL_ATOMP(STACK(base, 3)))
			{
				LOAD_SMSTR((CL_FORM *)&Kstruct_reader[4], STACK(base, 6));	/* ~s should be a list */
				COPY(STACK(base, 3), STACK(base, 7));
				Ferror(STACK(base, 6), 2);
			}
			COPY(STACK(base, 3), STACK(base, 6));
			Fcar(STACK(base, 6));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 6), CAR(lptr));
				COPY(STACK(base, 5), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 5));
			}
			COPY(STACK(base, 3), STACK(base, 6));
			Fcdr(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 3));
			goto M95_1;
			RETURN105:;
		}
	}
}
