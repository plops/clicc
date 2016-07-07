#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFdirectory)
{
	MAKE_STRING(1, "/"),	/* 0 */
};

static void Z67_g633(CL_FORM *base);
static void Z68_g629(CL_FORM *base);

void Fdirectory(CL_FORM *base, int nargs)
{
	BOOL supl_flags[3];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 230),	/* ALL */
		SYMBOL(Slisp, 231),	/* CHECK-FOR-SUBDIRS */
		SYMBOL(Slisp, 232),	/* FOLLOW-LINKS */
	};
	keysort(STACK(base, 1), nargs - 1, 3, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
	}
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 4));
	if(NOT(supl_flags[1]))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
	}
	GEN_HEAPVAR(STACK(base, 2), STACK(base, 4));
	if(NOT(supl_flags[2]))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
	}
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 4));
	LOAD_NIL(STACK(base, 4));
	GEN_HEAPVAR(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 185), STACK(base, 6));	/* NAME */
	LOAD_SYMBOL(SYMBOL(Slisp, 202), STACK(base, 7));	/* WILD */
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 8));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 202), STACK(base, 9));	/* WILD */
	LOAD_SYMBOL(SYMBOL(Slisp, 186), STACK(base, 10));	/* VERSION */
	LOAD_SYMBOL(SYMBOL(Slisp, 202), STACK(base, 11));	/* WILD */
	Fmake_pathname(STACK(base, 6), 6);
	Fmerge_pathnames(STACK(base, 5), 2);
	{
		GEN_CLOSURE(array, STACK(base, 6), 5, Z68_g629, 1);
		COPY(STACK(base, 4), &array[3]);
		COPY(STACK(base, 1), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 5), STACK(base, 7));
	Fpathname(STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
	COPY(STACK(base, 6), STACK(base, 9));
	Penumerate_matches(STACK(base, 7));
	mv_count = 1;
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	BIND_SPECIAL(SYMBOL(Slisp, 218), STACK(base, 5));	/* *IGNORE-WILDCARDS* */
	{
		GEN_CLOSURE(array, STACK(base, 6), 5, Z67_g633, 1);
		COPY(STACK(base, 3), &array[3]);
		COPY(STACK(base, 2), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	LOAD_NIL(STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 8));	/* TEST */
	LOAD_GLOBFUN(&CFstringE, STACK(base, 9));
	Fdelete_duplicates(STACK(base, 7), 3);
	LOAD_GLOBFUN(&CFstringL, STACK(base, 8));
	Fsort(STACK(base, 7), 2);
	Fmapcar(STACK(base, 6), 2);
	COPY(STACK(base, 6), STACK(base, 0));
	RESTORE_SPECIAL;
}

static void Z67_g633(CL_FORM *base)
{
	if(CL_TRUEP(INDIRECT(GET_FORM(STACK(base, 0)) + 4)))
	{
		COPY(STACK(base, 1), STACK(base, 2));
		unix_file_kind(STACK(base, 2), 1);
		bool_result = CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 184);	/* DIRECTORY */
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 2));	/* STRING */
		COPY(STACK(base, 1), STACK(base, 3));
		LOAD_SMSTR((CL_FORM *)&KFdirectory[0], STACK(base, 4));	/* / */
		Fconcatenate(STACK(base, 2), 3);
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 2));
	}
	if(CL_TRUEP(INDIRECT(GET_FORM(STACK(base, 0)) + 3)))
	{
		COPY(STACK(base, 2), STACK(base, 3));
		Ftruename(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 3));
		Fpathname(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 0));
	}
}

static void Z68_g629(CL_FORM *base)
{
	if(CL_TRUEP(INDIRECT(GET_FORM(STACK(base, 0)) + 4)))
	{
		bool_result = CL_TRUEP(INDIRECT(GET_FORM(STACK(base, 0)) + 4));
	}
	else
	{
		LOAD_CHAR('/', STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		LOAD_SYMBOL(SYMBOL(Slisp, 229), STACK(base, 4));	/* FROM-END */
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
		Fposition(STACK(base, 2), 4);
		if(NOT(CL_TRUEP(STACK(base, 2))))
		{
			LOAD_T(STACK(base, 3));
		}
		else
		{
			LOAD_NIL(STACK(base, 3));
		}
		if(CL_TRUEP(STACK(base, 3)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 4));
			F1plus(STACK(base, 4));
			COPY(STACK(base, 1), STACK(base, 5));
			Flength(STACK(base, 5));
			Fnumeql(STACK(base, 4), 2);
			if(CL_TRUEP(STACK(base, 4)))
			{
				bool_result = TRUE;
			}
			else
			{
				COPY(STACK(base, 1), STACK(base, 5));
				COPY(STACK(base, 2), STACK(base, 6));
				F1plus(STACK(base, 6));
				Fschar(STACK(base, 5));
				LOAD_CHAR('.', STACK(base, 6));
				FcharNE(STACK(base, 5), 2);
				bool_result = CL_TRUEP(STACK(base, 5));
			}
		}
	}
	if(bool_result)
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 2), 2);
		COPY(STACK(base, 1), CAR(lptr));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), CDR(lptr));
		LOAD_CONS(lptr, INDIRECT(GET_FORM(STACK(base, 0)) + 3));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
