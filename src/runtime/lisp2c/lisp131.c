#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KPenumerate_matches)
{
	MAKE_STRING(42, "Cannot supply a type without a name:~%  ~S"),	/* 0 */
	MAKE_NIL,	/* 2 CONS(2) */
	MAKE_LIST(&KPenumerate_matches[4]),
	MAKE_SYMREF(SYMBOL(Slisp, 197)),	/* CONS(4) UNSPECIFIC */
	MAKE_NIL,
	MAKE_STRING(45, "Cannot supply a version without a type:~%  ~S"),	/* 4 */
	MAKE_STRING(1, "/"),	/* 6 */
	MAKE_STRING(0, ""),	/* 8 */
	MAKE_STRING(40, "ecase: the value ~a is not a legal value"),	/* 10 */
	MAKE_STRING(0, ""),	/* 12 */
};

void Penumerate_matches(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	Fpathname_type(STACK(base, 3), 1);
	mv_count = 1;
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 0), STACK(base, 3));
		Fpathname_name(STACK(base, 3), 1);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 3)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KPenumerate_matches[0], STACK(base, 3));	/* Cannot supply a type without a name:~%  ~S */
			COPY(STACK(base, 0), STACK(base, 4));
			Ferror(STACK(base, 3), 2);
		}
	}
	COPY(STACK(base, 0), STACK(base, 3));
	Fpathname_version(STACK(base, 3));
	if(CL_FIXNUMP(STACK(base, 3)))
	{
		COPY(STACK(base, 0), STACK(base, 3));
		Fpathname_type(STACK(base, 3), 1);
		mv_count = 1;
		LOAD_CONS((CL_FORM *)&KPenumerate_matches[2], STACK(base, 4));
		Fmember(STACK(base, 3), 2);
		bool_result = CL_TRUEP(STACK(base, 3));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_SMSTR((CL_FORM *)&KPenumerate_matches[6], STACK(base, 3));	/* Cannot supply a version without a type:~%  ~S */
		COPY(STACK(base, 0), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	Fpathname_directory(STACK(base, 3), 1);
	mv_count = 1;
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 3), STACK(base, 4));
		Fcar(STACK(base, 4));
		if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 198))	/* ABSOLUTE */
		{
			LOAD_SMSTR((CL_FORM *)&KPenumerate_matches[8], STACK(base, 5));	/* / */
			COPY(STACK(base, 3), STACK(base, 6));
			Fcdr(STACK(base, 6));
			COPY(STACK(base, 0), STACK(base, 7));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
			COPY(STACK(base, 2), STACK(base, 9));
			Penumerate_directories(STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 0));
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 201))	/* RELATIVE */
			{
				LOAD_SMSTR((CL_FORM *)&KPenumerate_matches[10], STACK(base, 5));	/*  */
				COPY(STACK(base, 3), STACK(base, 6));
				Fcdr(STACK(base, 6));
				COPY(STACK(base, 0), STACK(base, 7));
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
				COPY(STACK(base, 2), STACK(base, 9));
				Penumerate_directories(STACK(base, 5));
				COPY(STACK(base, 5), STACK(base, 0));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KPenumerate_matches[12], STACK(base, 0));	/* ecase: the value ~a is not a legal value */
				COPY(STACK(base, 4), STACK(base, 1));
				Ferror(STACK(base, 0), 2);
			}
		}
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KPenumerate_matches[14], STACK(base, 4));	/*  */
		COPY(STACK(base, 0), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		COPY(STACK(base, 2), STACK(base, 7));
		Penumerate_files(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 0));
	}
}
