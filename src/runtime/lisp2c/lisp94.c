#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFparse_namestring)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(29, ":junk-allowed will be ignored"),	/* 2 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 4 */
	MAKE_STRING(30, "Hosts do not match: ~S and ~S."),	/* 6 */
};

void Fparse_namestring(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		COPY(SYMVAL(Slisp, 163), STACK(base, 2));	/* *DEFAULT-PATHNAME-DEFAULTS* */
		nargs = 3;
	}
	{
		BOOL supl_flags[3];
		static CL_FORM * keylist[] =
		{
			SYMBOL(Slisp, 161),	/* START */
			SYMBOL(Slisp, 162),	/* END */
			SYMBOL(Slisp, 195),	/* JUNK-ALLOWED */
		};
		keysort(STACK(base, 3), nargs - 3, 3, keylist, supl_flags, FALSE);
		if(NOT(supl_flags[0]))
		{
			LOAD_FIXNUM(0, STACK(base, 3));
		}
		if(NOT(supl_flags[1]))
		{
			LOAD_NIL(STACK(base, 4));
		}
		if(NOT(supl_flags[2]))
		{
			LOAD_NIL(STACK(base, 5));
		}
	}
	if(CL_STRING_P(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 3), STACK(base, 7));
		if(CL_TRUEP(STACK(base, 4)))
		{
			COPY(STACK(base, 4), STACK(base, 8));
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 8));
			Flength(STACK(base, 8));
		}
		if(CL_TRUEP(STACK(base, 1)))
		{
			COPY(STACK(base, 1), STACK(base, 9));
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 9));
			COPY(STACK(base, 2), STACK(base, 10));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 11));	/* PATHNAME */
			struct_typep(STACK(base, 10));
			if(CL_TRUEP(STACK(base, 10)))
			{
				COPY(STACK(base, 2), STACK(base, 9));
			}
			else
			{
				if(CL_ARRAY_P(STACK(base, 2)))
				{
					COPY(STACK(base, 2), STACK(base, 10));
					LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 11));	/* STANDARD-CHAR */
					LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 12));	/* * */
					check_array_internal(STACK(base, 10));
					bool_result = CL_TRUEP(STACK(base, 10));
				}
				else
				{
					bool_result = FALSE;
				}
				if(bool_result)
				{
					COPY(STACK(base, 2), STACK(base, 9));
					Fparse_namestring(STACK(base, 9), 1);
					mv_count = 1;
				}
				else
				{
					COPY(STACK(base, 2), STACK(base, 10));
					LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 11));	/* STREAM */
					struct_typep(STACK(base, 10));
					if(CL_TRUEP(STACK(base, 10)))
					{
						COPY(STACK(base, 2), STACK(base, 9));
						file_name(STACK(base, 9), 1);
						Fparse_namestring(STACK(base, 9), 1);
						mv_count = 1;
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&KFparse_namestring[0], STACK(base, 9));	/* etypecase: the value ~a is not a legal value */
						COPY(STACK(base, 2), STACK(base, 10));
						Ferror(STACK(base, 9), 2);
					}
				}
			}
			LOAD_FIXNUM(0, STACK(base, 10));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 11));	/* PATHNAME */
			struct_ref(STACK(base, 9));
		}
		COPY(STACK(base, 5), STACK(base, 10));
		M15_1:;
		if(CL_TRUEP(STACK(base, 10)))
		{
			LOAD_SMSTR((CL_FORM *)&KFparse_namestring[2], STACK(base, 0));	/* :junk-allowed will be ignored */
			Fwarn(STACK(base, 0), 1);
		}
		else
		{
			if(CL_SMSTRP(STACK(base, 6)))
			{
				COPY(STACK(base, 9), STACK(base, 11));
				host_parse(STACK(base, 11));
				COPY(STACK(base, 6), STACK(base, 12));
				COPY(STACK(base, 7), STACK(base, 13));
				COPY(STACK(base, 8), STACK(base, 14));
				Ffuncall(STACK(base, 11), 4);
				MV_TO_STACK(mv_count, 12);
				{
					int nargs;
					nargs = mv_count;
					mv_count = 1;
					switch(nargs)
					{
						case 0:
						LOAD_NIL(STACK(base, 11));
						case 1:
						LOAD_NIL(STACK(base, 12));
						case 2:
						LOAD_NIL(STACK(base, 13));
						case 3:
						LOAD_NIL(STACK(base, 14));
						case 4:
						LOAD_NIL(STACK(base, 15));
						case 5:
						LOAD_NIL(STACK(base, 16));
						nargs = 6;
					}
					Flist(STACK(base, 17), nargs - 6);
					if(CL_TRUEP(STACK(base, 11)))
					{
						COPY(STACK(base, 11), STACK(base, 18));
					}
					else
					{
						COPY(STACK(base, 9), STACK(base, 18));
					}
					LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 19));	/* PATHNAME */
					COPY(STACK(base, 18), STACK(base, 20));
					COPY(STACK(base, 12), STACK(base, 21));
					COPY(STACK(base, 13), STACK(base, 22));
					COPY(STACK(base, 14), STACK(base, 23));
					COPY(STACK(base, 15), STACK(base, 24));
					COPY(STACK(base, 16), STACK(base, 25));
					make_struct(STACK(base, 19), 7);
					COPY(STACK(base, 19), STACK(base, 18));
					COPY(STACK(base, 8), STACK(base, 19));
					COPY(STACK(base, 18), STACK(base, 0));
					COPY(STACK(base, 19), &mv_buf[0]);
					mv_count = 2;
				}
			}
			else
			{
				COPY(STACK(base, 6), STACK(base, 11));
				LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 12));	/* SIMPLE-STRING */
				Fcoerce(STACK(base, 11));
				COPY(STACK(base, 11), STACK(base, 6));
				LOAD_NIL(STACK(base, 10));
				goto M15_1;
			}
		}
		goto RETURN16;
		RETURN16:;
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 0), STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
		struct_typep(STACK(base, 7));
		if(CL_TRUEP(STACK(base, 7)))
		{
			COPY(STACK(base, 0), STACK(base, 6));
		}
		else
		{
			if(CL_ARRAY_P(STACK(base, 0)))
			{
				COPY(STACK(base, 0), STACK(base, 7));
				LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 8));	/* STANDARD-CHAR */
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 9));	/* * */
				check_array_internal(STACK(base, 7));
				bool_result = CL_TRUEP(STACK(base, 7));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 0), STACK(base, 6));
				Fparse_namestring(STACK(base, 6), 1);
				mv_count = 1;
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 7));
				LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 8));	/* STREAM */
				struct_typep(STACK(base, 7));
				if(CL_TRUEP(STACK(base, 7)))
				{
					COPY(STACK(base, 0), STACK(base, 6));
					file_name(STACK(base, 6), 1);
					Fparse_namestring(STACK(base, 6), 1);
					mv_count = 1;
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFparse_namestring[4], STACK(base, 6));	/* etypecase: the value ~a is not a legal value */
					COPY(STACK(base, 0), STACK(base, 7));
					Ferror(STACK(base, 6), 2);
				}
			}
		}
		if(CL_TRUEP(STACK(base, 1)))
		{
			COPY(STACK(base, 6), STACK(base, 7));
			LOAD_FIXNUM(0, STACK(base, 8));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 9));	/* PATHNAME */
			struct_ref(STACK(base, 7));
			if(EQ(STACK(base, 1), STACK(base, 7)))
			{
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFparse_namestring[6], STACK(base, 7));	/* Hosts do not match: ~S and ~S. */
				COPY(STACK(base, 1), STACK(base, 8));
				COPY(STACK(base, 6), STACK(base, 9));
				LOAD_FIXNUM(0, STACK(base, 10));
				LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 11));	/* PATHNAME */
				struct_ref(STACK(base, 9));
				Ferror(STACK(base, 7), 3);
			}
		}
		COPY(STACK(base, 6), STACK(base, 7));
		COPY(STACK(base, 3), STACK(base, 8));
		COPY(STACK(base, 7), STACK(base, 0));
		COPY(STACK(base, 8), &mv_buf[0]);
		mv_count = 2;
	}
}
