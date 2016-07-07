#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFpathname_match_p)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 2 */
};

void Fpathname_match_p(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 4));	/* PATHNAME */
	struct_typep(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
	}
	else
	{
		if(CL_ARRAY_P(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 4));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 5));	/* * */
			check_array_internal(STACK(base, 3));
			bool_result = CL_TRUEP(STACK(base, 3));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fparse_namestring(STACK(base, 2), 1);
			mv_count = 1;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 4));	/* STREAM */
			struct_typep(STACK(base, 3));
			if(CL_TRUEP(STACK(base, 3)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				file_name(STACK(base, 2), 1);
				Fparse_namestring(STACK(base, 2), 1);
				mv_count = 1;
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFpathname_match_p[0], STACK(base, 2));	/* etypecase: the value ~a is not a legal value */
				COPY(STACK(base, 0), STACK(base, 3));
				Ferror(STACK(base, 2), 2);
			}
		}
	}
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
	struct_typep(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 3));
	}
	else
	{
		if(CL_ARRAY_P(STACK(base, 1)))
		{
			COPY(STACK(base, 1), STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 5));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
			check_array_internal(STACK(base, 4));
			bool_result = CL_TRUEP(STACK(base, 4));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 1), STACK(base, 3));
			Fparse_namestring(STACK(base, 3), 1);
			mv_count = 1;
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 5));	/* STREAM */
			struct_typep(STACK(base, 4));
			if(CL_TRUEP(STACK(base, 4)))
			{
				COPY(STACK(base, 1), STACK(base, 3));
				file_name(STACK(base, 3), 1);
				Fparse_namestring(STACK(base, 3), 1);
				mv_count = 1;
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFpathname_match_p[2], STACK(base, 3));	/* etypecase: the value ~a is not a legal value */
				COPY(STACK(base, 1), STACK(base, 4));
				Ferror(STACK(base, 3), 2);
			}
		}
	}
	COPY(STACK(base, 3), STACK(base, 4));
	LOAD_FIXNUM(0, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
	struct_ref(STACK(base, 4));
	if(NOT(CL_TRUEP(STACK(base, 4))))
	{
		LOAD_T(STACK(base, 4));
	}
	else
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(CL_TRUEP(STACK(base, 4)))
	{
		bool_result = TRUE;
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 5));
		LOAD_FIXNUM(0, STACK(base, 6));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
		struct_ref(STACK(base, 5));
		COPY(STACK(base, 3), STACK(base, 6));
		LOAD_FIXNUM(0, STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
		struct_ref(STACK(base, 6));
		components_match(STACK(base, 5));
		bool_result = CL_TRUEP(STACK(base, 5));
	}
	if(bool_result)
	{
		COPY(STACK(base, 3), STACK(base, 4));
		LOAD_FIXNUM(1, STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
		struct_ref(STACK(base, 4));
		if(NOT(CL_TRUEP(STACK(base, 4))))
		{
			LOAD_T(STACK(base, 4));
		}
		else
		{
			LOAD_NIL(STACK(base, 4));
		}
		if(CL_TRUEP(STACK(base, 4)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 5));
			LOAD_FIXNUM(1, STACK(base, 6));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
			struct_ref(STACK(base, 5));
			COPY(STACK(base, 3), STACK(base, 6));
			LOAD_FIXNUM(1, STACK(base, 7));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
			struct_ref(STACK(base, 6));
			components_match(STACK(base, 5));
			bool_result = CL_TRUEP(STACK(base, 5));
		}
		if(bool_result)
		{
			COPY(STACK(base, 3), STACK(base, 4));
			LOAD_FIXNUM(2, STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
			struct_ref(STACK(base, 4));
			if(NOT(CL_TRUEP(STACK(base, 4))))
			{
				LOAD_T(STACK(base, 4));
			}
			else
			{
				LOAD_NIL(STACK(base, 4));
			}
			if(CL_TRUEP(STACK(base, 4)))
			{
				bool_result = TRUE;
			}
			else
			{
				COPY(STACK(base, 2), STACK(base, 5));
				LOAD_FIXNUM(2, STACK(base, 6));
				LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
				struct_ref(STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				LOAD_FIXNUM(2, STACK(base, 7));
				LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
				struct_ref(STACK(base, 6));
				components_match(STACK(base, 5));
				bool_result = CL_TRUEP(STACK(base, 5));
			}
			if(bool_result)
			{
				COPY(STACK(base, 3), STACK(base, 4));
				LOAD_FIXNUM(3, STACK(base, 5));
				LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
				struct_ref(STACK(base, 4));
				if(NOT(CL_TRUEP(STACK(base, 4))))
				{
					LOAD_T(STACK(base, 4));
				}
				else
				{
					LOAD_NIL(STACK(base, 4));
				}
				if(CL_TRUEP(STACK(base, 4)))
				{
					bool_result = TRUE;
				}
				else
				{
					COPY(STACK(base, 2), STACK(base, 5));
					LOAD_FIXNUM(3, STACK(base, 6));
					LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
					struct_ref(STACK(base, 5));
					COPY(STACK(base, 3), STACK(base, 6));
					LOAD_FIXNUM(3, STACK(base, 7));
					LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
					struct_ref(STACK(base, 6));
					components_match(STACK(base, 5));
					bool_result = CL_TRUEP(STACK(base, 5));
				}
				if(bool_result)
				{
					COPY(STACK(base, 3), STACK(base, 4));
					LOAD_FIXNUM(4, STACK(base, 5));
					LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
					struct_ref(STACK(base, 4));
					if(NOT(CL_TRUEP(STACK(base, 4))))
					{
						LOAD_T(STACK(base, 4));
					}
					else
					{
						LOAD_NIL(STACK(base, 4));
					}
					if(CL_TRUEP(STACK(base, 4)))
					{
						bool_result = TRUE;
					}
					else
					{
						COPY(STACK(base, 2), STACK(base, 5));
						LOAD_FIXNUM(4, STACK(base, 6));
						LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
						struct_ref(STACK(base, 5));
						COPY(STACK(base, 3), STACK(base, 6));
						LOAD_FIXNUM(4, STACK(base, 7));
						LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
						struct_ref(STACK(base, 6));
						components_match(STACK(base, 5));
						bool_result = CL_TRUEP(STACK(base, 5));
					}
					if(bool_result)
					{
						COPY(STACK(base, 3), STACK(base, 4));
						LOAD_FIXNUM(5, STACK(base, 5));
						LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
						struct_ref(STACK(base, 4));
						if(NOT(CL_TRUEP(STACK(base, 4))))
						{
							LOAD_T(STACK(base, 4));
						}
						else
						{
							LOAD_NIL(STACK(base, 4));
						}
						if(CL_TRUEP(STACK(base, 4)))
						{
							LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
						}
						else
						{
							COPY(STACK(base, 3), STACK(base, 5));
							LOAD_FIXNUM(5, STACK(base, 6));
							LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
							struct_ref(STACK(base, 5));
							if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 202))	/* WILD */
							{
								LOAD_T(STACK(base, 5));
							}
							else
							{
								LOAD_NIL(STACK(base, 5));
							}
							if(CL_TRUEP(STACK(base, 5)))
							{
								LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
							}
							else
							{
								COPY(STACK(base, 2), STACK(base, 6));
								LOAD_FIXNUM(5, STACK(base, 7));
								LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 8));	/* PATHNAME */
								struct_ref(STACK(base, 6));
								COPY(STACK(base, 3), STACK(base, 7));
								LOAD_FIXNUM(5, STACK(base, 8));
								LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 9));	/* PATHNAME */
								struct_ref(STACK(base, 7));
								if(EQ(STACK(base, 6), STACK(base, 7)) || CL_FLOATP(STACK(base, 6)) && GET_FLOAT(STACK(base, 6)) == GET_FLOAT(STACK(base, 7)))
								{
									LOAD_T(STACK(base, 0));
								}
								else
								{
									LOAD_NIL(STACK(base, 0));
								}
							}
						}
					}
					else
					{
						LOAD_NIL(STACK(base, 0));
					}
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
				}
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
