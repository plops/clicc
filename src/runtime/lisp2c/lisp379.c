/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

extern CL_INIT XIunparse_unix_enough[];
extern CL_INIT XFunparse_unix_enough[];
CL_INIT Kunparse_unix_enough[] =
{
	MAKE_STRING(1, "."),	/* 0 */
	MAKE_STRING(2, ".*"),	/* 2 */
	MAKE_STRING(3, ".~D"),	/* 4 */
};

void unparse_unix_enough(CL_FORM *base)
{
	LOAD_NIL(ARG(2));
	LOAD_NIL(ARG(3));
	ALLOC_CONS(ARG(4), ARG(2), ARG(3), ARG(2));
	COPY(ARG(0), ARG(3));
	Ppathname_directory(ARG(3));
	COPY(ARG(1), ARG(4));
	Ppathname_directory(ARG(4));
	COPY(ARG(4), ARG(5));
	Flength(ARG(5));
	COPY(ARG(5), ARG(6));
	LOAD_SMALLFIXNUM(1, ARG(7));
	Fgt(ARG(6), 2);
	if(CL_TRUEP(ARG(6)))
	{
		COPY(ARG(3), ARG(6));
		Flength(ARG(6));
		COPY(ARG(5), ARG(7));
		Fge(ARG(6), 2);
		if(CL_TRUEP(ARG(6)))
		{
			LOAD_SMALLFIXNUM(0, ARG(6));
			if(CL_LISTP(ARG(3)))
			{
				COPY(ARG(3), ARG(7));
				COPY(ARG(6), ARG(8));
				COPY(ARG(5), ARG(9));
				list_subseq(ARG(7));
				COPY(ARG(7), ARG(6));
			}
			else
			{
				COPY(ARG(3), ARG(7));
				COPY(ARG(6), ARG(8));
				COPY(ARG(5), ARG(9));
				vector_subseq(ARG(7));
				COPY(ARG(7), ARG(6));
			}
			COPY(ARG(4), ARG(7));
			compare_component(ARG(6));
		}
		else
		{
			goto ELSE1;
		}
	}
	else
	{
		goto ELSE1;
	}
	if(CL_TRUEP(ARG(6)))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 261), ARG(6));	/* RELATIVE */
		COPY(ARG(5), ARG(7));
		COPY(ARG(3), ARG(8));
		Fnthcdr(ARG(7));
		ALLOC_CONS(ARG(8), ARG(6), ARG(7), ARG(6));
	}
	else
	{
		ELSE1:;
		if(CL_CONSP(ARG(3)))
		{
			COPY(GET_CAR(ARG(3)), ARG(6));
		}
		else
		{
			if(CL_TRUEP(ARG(3)))
			{
				LOAD_SMSTR((CL_FORM *)&KClisp[239], ARG(6));	/* ~a is not a list */
				COPY(ARG(3), ARG(7));
				Ferror(ARG(6), 2);
			}
			else
			{
				COPY(ARG(3), ARG(6));
			}
		}
		if(CL_SYMBOLP(ARG(6)) && GET_SYMBOL(ARG(6)) == SYMBOL(Slisp, 257))	/* ABSOLUTE */
		{
			COPY(ARG(3), ARG(6));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[210], ARG(6));	/* ~S cannot be represented relative to ~S */
			COPY(ARG(0), ARG(7));
			COPY(ARG(1), ARG(8));
			Ferror(ARG(6), 3);
		}
	}
	COPY(ARG(6), ARG(7));
	unparse_unix_directory_list(ARG(7));
	COPY(ARG(2), ARG(8));
	add_q(ARG(7));
	COPY(ARG(0), ARG(3));
	Ppathname_version(ARG(3));
	if(CL_TRUEP(ARG(3)))
	{
		if(CL_SYMBOLP(ARG(3)) && GET_SYMBOL(ARG(3)) == SYMBOL(Slisp, 259))	/* NEWEST */
		{
			LOAD_NIL(ARG(4));
		}
		else
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(4));	/* T */
		}
	}
	else
	{
		LOAD_NIL(ARG(4));
	}
	COPY(ARG(0), ARG(5));
	Ppathname_type(ARG(5));
	if(CL_TRUEP(ARG(4)))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(6));	/* T */
	}
	else
	{
		if(CL_TRUEP(ARG(5)))
		{
			if(CL_SYMBOLP(ARG(5)) && GET_SYMBOL(ARG(5)) == SYMBOL(Slisp, 256))	/* UNSPECIFIC */
			{
				LOAD_NIL(ARG(6));
			}
			else
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(6));	/* T */
			}
		}
		else
		{
			LOAD_NIL(ARG(6));
		}
	}
	COPY(ARG(0), ARG(7));
	Ppathname_name(ARG(7));
	if(CL_TRUEP(ARG(6)))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(8));	/* T */
	}
	else
	{
		if(CL_TRUEP(ARG(7)))
		{
			COPY(ARG(7), ARG(8));
			COPY(ARG(1), ARG(9));
			Ppathname_name(ARG(9));
			compare_component(ARG(8));
			if(CL_TRUEP(ARG(8)))
			{
				LOAD_NIL(ARG(8));
			}
			else
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(8));	/* T */
			}
		}
		else
		{
			LOAD_NIL(ARG(8));
		}
	}
	if(CL_TRUEP(ARG(8)))
	{
		if(CL_TRUEP(ARG(7)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[210], ARG(9));	/* ~S cannot be represented relative to ~S */
			COPY(ARG(0), ARG(10));
			COPY(ARG(1), ARG(11));
			Ferror(ARG(9), 3);
		}
		COPY(ARG(7), ARG(9));
		unparse_unix_piece(ARG(9));
		COPY(ARG(2), ARG(10));
		add_q(ARG(9));
	}
	if(CL_TRUEP(ARG(6)))
	{
		if(CL_TRUEP(ARG(5)))
		{
			LOAD_NIL(ARG(9));
		}
		else
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(9));	/* T */
		}
		if(CL_TRUEP(ARG(9)))
		{
			goto THEN2;
		}
		else
		{
		}	/* UNSPECIFIC */
		if(CL_SYMBOLP(ARG(5)) && GET_SYMBOL(ARG(5)) == SYMBOL(Slisp, 256))
		{
			THEN2:;
			LOAD_SMSTR((CL_FORM *)&KClisp[210], ARG(9));	/* ~S cannot be represented relative to ~S */
			COPY(ARG(0), ARG(10));
			COPY(ARG(1), ARG(11));
			Ferror(ARG(9), 3);
		}
		LOAD_SMSTR((CL_FORM *)&Kunparse_unix_enough[0], ARG(9));	/* . */
		COPY(ARG(2), ARG(10));
		add_q(ARG(9));
		COPY(ARG(5), ARG(9));
		unparse_unix_piece(ARG(9));
		COPY(ARG(2), ARG(10));
		add_q(ARG(9));
	}
	if(CL_TRUEP(ARG(4)))
	{
		if(CL_SYMBOLP(ARG(3)) && GET_SYMBOL(ARG(3)) == SYMBOL(Slisp, 262))	/* WILD */
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_enough[2], ARG(9));	/* .* */
			COPY(ARG(2), ARG(10));
			add_q(ARG(9));
		}
		else
		{
			if(CL_FIXNUMP(ARG(3)))
			{
				LOAD_NIL(ARG(9));
				LOAD_SMSTR((CL_FORM *)&Kunparse_unix_enough[4], ARG(10));	/* .~D */
				COPY(ARG(3), ARG(11));
				Fformat(ARG(9), 3);
				mv_count = 1;
				COPY(ARG(2), ARG(10));
				add_q(ARG(9));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KClisp[210], ARG(9));	/* ~S cannot be represented relative to ~S */
				COPY(ARG(0), ARG(10));
				COPY(ARG(1), ARG(11));
				Ferror(ARG(9), 3);
			}
		}
	}
	LOAD_GLOBFUN(&CFconcatenate, ARG(0));
	LOAD_SYMBOL(SYMBOL(Slisp, 40), ARG(1));	/* SIMPLE-STRING */
	COPY(GET_CAR(ARG(2)), ARG(2));
	Fapply(ARG(0), 3);
}
