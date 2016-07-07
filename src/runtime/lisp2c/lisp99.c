#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_pathname)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(40, "ecase: the value ~a is not a legal value"),	/* 2 */
	MAKE_STRING(30, ":WILD-INFERIORS not supported."),	/* 4 */
	MAKE_SYMREF(SYMBOL(Slisp, 205)),	/* 6 CONS(6) UP */
	MAKE_LIST(&KFmake_pathname[8]),
	MAKE_SYMREF(SYMBOL(Slisp, 199)),	/* CONS(8) BACK */
	MAKE_NIL,
	MAKE_STRING(43, "~S is not allowed as a directory component."),	/* 8 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 10 */
};

void Fmake_pathname(CL_FORM *base, int nargs)
{
	BOOL supl_flags[8];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 182),	/* HOST */
		SYMBOL(Slisp, 183),	/* DEVICE */
		SYMBOL(Slisp, 184),	/* DIRECTORY */
		SYMBOL(Slisp, 185),	/* NAME */
		SYMBOL(Slisp, 79),	/* TYPE */
		SYMBOL(Slisp, 186),	/* VERSION */
		SYMBOL(Slisp, 206),	/* DEFAULTS */
		SYMBOL(Slisp, 208),	/* CASE */
	};
	keysort(STACK(base, 0), nargs - 0, 8, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 0));
		LOAD_NIL(STACK(base, 8));
	}
	else
	{
		LOAD_T(STACK(base, 8));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 1));
		LOAD_NIL(STACK(base, 9));
	}
	else
	{
		LOAD_T(STACK(base, 9));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 2));
		LOAD_NIL(STACK(base, 10));
	}
	else
	{
		LOAD_T(STACK(base, 10));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_NIL(STACK(base, 3));
		LOAD_NIL(STACK(base, 11));
	}
	else
	{
		LOAD_T(STACK(base, 11));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 4));
		LOAD_NIL(STACK(base, 12));
	}
	else
	{
		LOAD_T(STACK(base, 12));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 5));
		LOAD_NIL(STACK(base, 13));
	}
	else
	{
		LOAD_T(STACK(base, 13));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_NIL(STACK(base, 6));
	}
	if(NOT(supl_flags[7]))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 207), STACK(base, 7));	/* LOCAL */
	}
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 6), STACK(base, 14));
		COPY(STACK(base, 6), STACK(base, 15));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 16));	/* PATHNAME */
		struct_typep(STACK(base, 15));
		if(CL_TRUEP(STACK(base, 15)))
		{
			COPY(STACK(base, 6), STACK(base, 14));
		}
		else
		{
			if(CL_ARRAY_P(STACK(base, 6)))
			{
				COPY(STACK(base, 6), STACK(base, 15));
				LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 16));	/* STANDARD-CHAR */
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 17));	/* * */
				check_array_internal(STACK(base, 15));
				bool_result = CL_TRUEP(STACK(base, 15));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 6), STACK(base, 14));
				Fparse_namestring(STACK(base, 14), 1);
				mv_count = 1;
			}
			else
			{
				COPY(STACK(base, 6), STACK(base, 15));
				LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 16));	/* STREAM */
				struct_typep(STACK(base, 15));
				if(CL_TRUEP(STACK(base, 15)))
				{
					COPY(STACK(base, 6), STACK(base, 14));
					file_name(STACK(base, 14), 1);
					Fparse_namestring(STACK(base, 14), 1);
					mv_count = 1;
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFmake_pathname[0], STACK(base, 14));	/* etypecase: the value ~a is not a legal value */
					COPY(STACK(base, 6), STACK(base, 15));
					Ferror(STACK(base, 14), 2);
				}
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 14));
	}
	if(CL_TRUEP(STACK(base, 14)))
	{
		COPY(STACK(base, 14), STACK(base, 15));
		LOAD_FIXNUM(0, STACK(base, 16));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 17));	/* PATHNAME */
		struct_ref(STACK(base, 15));
	}
	else
	{
		COPY(SYMVAL(Slisp, 163), STACK(base, 15));	/* *DEFAULT-PATHNAME-DEFAULTS* */
		Fpathname_host(STACK(base, 15), 1);
	}
	if(CL_TRUEP(STACK(base, 8)))
	{
		COPY(STACK(base, 0), STACK(base, 16));
	}
	else
	{
		COPY(STACK(base, 15), STACK(base, 16));
	}
	if(CL_SYMBOLP(STACK(base, 7)) && GET_SYMBOL(STACK(base, 7)) == SYMBOL(Slisp, 209))	/* COMMON */
	{
		COPY(STACK(base, 16), STACK(base, 17));
		LOAD_FIXNUM(6, STACK(base, 18));
		LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 19));	/* HOST */
		struct_ref(STACK(base, 17));
		if(CL_SYMBOLP(STACK(base, 17)) && GET_SYMBOL(STACK(base, 17)) == SYMBOL(Slisp, 210))	/* LOWER */
		{
			LOAD_T(STACK(base, 17));
		}
		else
		{
			LOAD_NIL(STACK(base, 17));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 17));
	}
	COPY(STACK(base, 16), STACK(base, 18));
	LOAD_FIXNUM(6, STACK(base, 19));
	LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 20));	/* HOST */
	struct_ref(STACK(base, 18));
	COPY(STACK(base, 15), STACK(base, 19));
	LOAD_FIXNUM(6, STACK(base, 20));
	LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 21));	/* HOST */
	struct_ref(STACK(base, 19));
	if(NOT(EQ(STACK(base, 18), STACK(base, 19))))
	{
		LOAD_T(STACK(base, 18));
	}
	else
	{
		LOAD_NIL(STACK(base, 18));
	}
	COPY(STACK(base, 16), STACK(base, 19));
	if(CL_TRUEP(STACK(base, 9)))
	{
		COPY(STACK(base, 1), STACK(base, 20));
	}
	else
	{
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 14), STACK(base, 20));
			LOAD_FIXNUM(1, STACK(base, 21));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 22));	/* PATHNAME */
			struct_ref(STACK(base, 20));
		}
		else
		{
			LOAD_NIL(STACK(base, 20));
		}
	}
	COPY(STACK(base, 2), STACK(base, 21));
	COPY(STACK(base, 17), STACK(base, 22));
	if(CL_TRUEP(STACK(base, 21)))
	{
		if(CL_LISTP(STACK(base, 21)))
		{
			LOAD_NIL(STACK(base, 23));
			LOAD_NIL(STACK(base, 24));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 25), 2);
				COPY(STACK(base, 23), CAR(lptr));
				COPY(STACK(base, 24), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 23));
			}
			COPY(STACK(base, 21), STACK(base, 24));
			Fcar(STACK(base, 24));
			COPY(STACK(base, 21), STACK(base, 25));
			COPY(STACK(base, 25), STACK(base, 26));
			Fcdr(STACK(base, 26));
			COPY(STACK(base, 26), STACK(base, 21));
			if(CL_SYMBOLP(STACK(base, 24)) && GET_SYMBOL(STACK(base, 24)) == SYMBOL(Slisp, 198))	/* ABSOLUTE */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 198), STACK(base, 25));	/* ABSOLUTE */
				COPY(STACK(base, 23), STACK(base, 26));
				add_q(STACK(base, 25));
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 24)) && GET_SYMBOL(STACK(base, 24)) == SYMBOL(Slisp, 201))	/* RELATIVE */
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 201), STACK(base, 25));	/* RELATIVE */
					COPY(STACK(base, 23), STACK(base, 26));
					add_q(STACK(base, 25));
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFmake_pathname[2], STACK(base, 25));	/* ecase: the value ~a is not a legal value */
					COPY(STACK(base, 24), STACK(base, 26));
					Ferror(STACK(base, 25), 2);
				}
			}
			LOAD_NIL(STACK(base, 24));
			COPY(STACK(base, 21), STACK(base, 25));
			M21_1:;
			if(CL_ATOMP(STACK(base, 25)))
			{
				LOAD_NIL(STACK(base, 24));
				goto RETURN22;
			}
			COPY(STACK(base, 25), STACK(base, 26));
			Fcar(STACK(base, 26));
			COPY(STACK(base, 26), STACK(base, 24));
			if(CL_SYMBOLP(STACK(base, 24)) && GET_SYMBOL(STACK(base, 24)) == SYMBOL(Slisp, 202))	/* WILD */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 203), STACK(base, 26));	/* MULTI-CHAR-WILD */
				Flist(STACK(base, 26), 1);
				make_pattern(STACK(base, 26), 1);
				COPY(STACK(base, 23), STACK(base, 27));
				add_q(STACK(base, 26));
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 24)) && GET_SYMBOL(STACK(base, 24)) == SYMBOL(Slisp, 204))	/* WILD-INFERIORS */
				{
					LOAD_SMSTR((CL_FORM *)&KFmake_pathname[4], STACK(base, 26));	/* :WILD-INFERIORS not supported. */
					Ferror(STACK(base, 26), 1);
				}
				else
				{
					COPY(STACK(base, 24), STACK(base, 26));
					LOAD_CONS((CL_FORM *)&KFmake_pathname[6], STACK(base, 27));
					Fmember(STACK(base, 26), 2);
					if(CL_TRUEP(STACK(base, 26)))
					{
						COPY(STACK(base, 24), STACK(base, 26));
						COPY(STACK(base, 23), STACK(base, 27));
						add_q(STACK(base, 26));
					}
					else
					{
						if(CL_SMSTRP(STACK(base, 24)))
						{
							LOAD_T(STACK(base, 26));
						}
						else
						{
							LOAD_NIL(STACK(base, 26));
						}
						if(CL_TRUEP(STACK(base, 26)))
						{
							bool_result = TRUE;
						}
						else
						{
							COPY(STACK(base, 24), STACK(base, 27));
							LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 28));	/* PATTERN */
							struct_typep(STACK(base, 27));
							bool_result = CL_TRUEP(STACK(base, 27));
						}
						if(bool_result)
						{
							COPY(STACK(base, 24), STACK(base, 26));
							COPY(STACK(base, 22), STACK(base, 27));
							maybe_diddle_case(STACK(base, 26));
							mv_count = 1;
							COPY(STACK(base, 23), STACK(base, 27));
							add_q(STACK(base, 26));
						}
						else
						{
							if(CL_STRING_P(STACK(base, 24)))
							{
								COPY(STACK(base, 24), STACK(base, 26));
								LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 27));	/* SIMPLE-STRING */
								Fcoerce(STACK(base, 26));
								COPY(STACK(base, 22), STACK(base, 27));
								maybe_diddle_case(STACK(base, 26));
								mv_count = 1;
								COPY(STACK(base, 23), STACK(base, 27));
								add_q(STACK(base, 26));
							}
							else
							{
								LOAD_SMSTR((CL_FORM *)&KFmake_pathname[10], STACK(base, 26));	/* ~S is not allowed as a directory component. */
								COPY(STACK(base, 24), STACK(base, 27));
								Ferror(STACK(base, 26), 2);
							}
						}
					}
				}
			}
			Fcdr(STACK(base, 25));
			goto M21_1;
			RETURN22:;
			COPY(STACK(base, 23), STACK(base, 21));
			Fcar(STACK(base, 21));
		}
		else
		{
			if(CL_SMAR_P(STACK(base, 21)))
			{
				COPY(STACK(base, 21), STACK(base, 23));
				LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 24));	/* STANDARD-CHAR */
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 25));	/* * */
				check_array_internal(STACK(base, 23));
				bool_result = CL_TRUEP(STACK(base, 23));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 198), STACK(base, 23));	/* ABSOLUTE */
				COPY(STACK(base, 21), STACK(base, 24));
				COPY(STACK(base, 22), STACK(base, 25));
				maybe_diddle_case(STACK(base, 24));
				mv_count = 1;
				Flist(STACK(base, 23), 2);
				COPY(STACK(base, 23), STACK(base, 21));
			}
			else
			{
				if(CL_ARRAY_P(STACK(base, 21)))
				{
					COPY(STACK(base, 21), STACK(base, 23));
					LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 24));	/* STANDARD-CHAR */
					LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 25));	/* * */
					check_array_internal(STACK(base, 23));
					bool_result = CL_TRUEP(STACK(base, 23));
				}
				else
				{
					bool_result = FALSE;
				}
				if(bool_result)
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 198), STACK(base, 23));	/* ABSOLUTE */
					COPY(STACK(base, 21), STACK(base, 24));
					LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 25));	/* SIMPLE-STRING */
					Fcoerce(STACK(base, 24));
					COPY(STACK(base, 22), STACK(base, 25));
					maybe_diddle_case(STACK(base, 24));
					mv_count = 1;
					Flist(STACK(base, 23), 2);
					COPY(STACK(base, 23), STACK(base, 21));
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFmake_pathname[12], STACK(base, 23));	/* etypecase: the value ~a is not a legal value */
					COPY(STACK(base, 21), STACK(base, 24));
					Ferror(STACK(base, 23), 2);
				}
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 21));
	}
	if(CL_TRUEP(STACK(base, 14)))
	{
		bool_result = NOT(CL_TRUEP(STACK(base, 10)));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(STACK(base, 14), STACK(base, 22));
		LOAD_FIXNUM(2, STACK(base, 23));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 24));	/* PATHNAME */
		struct_ref(STACK(base, 22));
		COPY(STACK(base, 18), STACK(base, 23));
		merge_directories(STACK(base, 21));
	}
	else
	{
	}
	if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 202))	/* WILD */
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 203), STACK(base, 22));	/* MULTI-CHAR-WILD */
		Flist(STACK(base, 22), 1);
		make_pattern(STACK(base, 22), 1);
	}
	else
	{
		if(CL_SMSTRP(STACK(base, 3)))
		{
			LOAD_T(STACK(base, 22));
		}
		else
		{
			LOAD_NIL(STACK(base, 22));
		}
		if(CL_TRUEP(STACK(base, 22)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 3), STACK(base, 23));
			LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 24));	/* PATTERN */
			struct_typep(STACK(base, 23));
			bool_result = CL_TRUEP(STACK(base, 23));
		}
		if(bool_result)
		{
			COPY(STACK(base, 3), STACK(base, 22));
			COPY(STACK(base, 17), STACK(base, 23));
			maybe_diddle_case(STACK(base, 22));
			mv_count = 1;
		}
		else
		{
			if(CL_STRING_P(STACK(base, 3)))
			{
				COPY(STACK(base, 3), STACK(base, 22));
				LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 23));	/* SIMPLE-STRING */
				Fcoerce(STACK(base, 22));
				COPY(STACK(base, 17), STACK(base, 23));
				maybe_diddle_case(STACK(base, 22));
				mv_count = 1;
			}
			else
			{
				if(CL_TRUEP(STACK(base, 11)))
				{
					COPY(STACK(base, 3), STACK(base, 22));
					COPY(STACK(base, 17), STACK(base, 23));
					maybe_diddle_case(STACK(base, 22));
					mv_count = 1;
				}
				else
				{
					if(CL_TRUEP(STACK(base, 14)))
					{
						COPY(STACK(base, 14), STACK(base, 22));
						LOAD_FIXNUM(3, STACK(base, 23));
						LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 24));	/* PATHNAME */
						struct_ref(STACK(base, 22));
						COPY(STACK(base, 18), STACK(base, 23));
						maybe_diddle_case(STACK(base, 22));
						mv_count = 1;
					}
					else
					{
						LOAD_NIL(STACK(base, 22));
					}
				}
			}
		}
	}
	if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 202))	/* WILD */
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 203), STACK(base, 23));	/* MULTI-CHAR-WILD */
		Flist(STACK(base, 23), 1);
		make_pattern(STACK(base, 23), 1);
	}
	else
	{
		if(CL_SMSTRP(STACK(base, 4)))
		{
			LOAD_T(STACK(base, 23));
		}
		else
		{
			LOAD_NIL(STACK(base, 23));
		}
		if(CL_TRUEP(STACK(base, 23)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 4), STACK(base, 24));
			LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 25));	/* PATTERN */
			struct_typep(STACK(base, 24));
			bool_result = CL_TRUEP(STACK(base, 24));
		}
		if(bool_result)
		{
			COPY(STACK(base, 4), STACK(base, 23));
			COPY(STACK(base, 17), STACK(base, 24));
			maybe_diddle_case(STACK(base, 23));
			mv_count = 1;
		}
		else
		{
			if(CL_STRING_P(STACK(base, 4)))
			{
				COPY(STACK(base, 4), STACK(base, 23));
				LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 24));	/* SIMPLE-STRING */
				Fcoerce(STACK(base, 23));
				COPY(STACK(base, 17), STACK(base, 24));
				maybe_diddle_case(STACK(base, 23));
				mv_count = 1;
			}
			else
			{
				if(CL_TRUEP(STACK(base, 12)))
				{
					COPY(STACK(base, 4), STACK(base, 23));
					COPY(STACK(base, 17), STACK(base, 24));
					maybe_diddle_case(STACK(base, 23));
					mv_count = 1;
				}
				else
				{
					if(CL_TRUEP(STACK(base, 14)))
					{
						COPY(STACK(base, 14), STACK(base, 23));
						LOAD_FIXNUM(4, STACK(base, 24));
						LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 25));	/* PATHNAME */
						struct_ref(STACK(base, 23));
						COPY(STACK(base, 18), STACK(base, 24));
						maybe_diddle_case(STACK(base, 23));
						mv_count = 1;
					}
					else
					{
						LOAD_NIL(STACK(base, 23));
					}
				}
			}
		}
	}
	if(CL_TRUEP(STACK(base, 13)))
	{
		COPY(STACK(base, 5), STACK(base, 24));
	}
	else
	{
		if(CL_TRUEP(STACK(base, 14)))
		{
			COPY(STACK(base, 14), STACK(base, 24));
			LOAD_FIXNUM(5, STACK(base, 25));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 26));	/* PATHNAME */
			struct_ref(STACK(base, 24));
		}
		else
		{
			LOAD_NIL(STACK(base, 24));
		}
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 0));	/* PATHNAME */
	COPY(STACK(base, 19), STACK(base, 1));
	COPY(STACK(base, 20), STACK(base, 2));
	COPY(STACK(base, 21), STACK(base, 3));
	COPY(STACK(base, 22), STACK(base, 4));
	COPY(STACK(base, 23), STACK(base, 5));
	COPY(STACK(base, 24), STACK(base, 6));
	make_struct(STACK(base, 0), 7);
}
