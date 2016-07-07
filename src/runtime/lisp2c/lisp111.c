#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFwild_pathname_p)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(40, "ecase: the value ~a is not a legal value"),	/* 2 */
};

void Fwild_pathname_p(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
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
				LOAD_SMSTR((CL_FORM *)&KFwild_pathname_p[0], STACK(base, 2));	/* etypecase: the value ~a is not a legal value */
				COPY(STACK(base, 0), STACK(base, 3));
				Ferror(STACK(base, 2), 2);
			}
		}
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 182))	/* HOST */
		{
			COPY(STACK(base, 2), STACK(base, 3));
			LOAD_FIXNUM(0, STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
			struct_ref(STACK(base, 3));
			COPY(STACK(base, 3), STACK(base, 0));
			LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 1));	/* PATTERN */
			struct_typep(STACK(base, 0));
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 183))	/* DEVICE */
			{
				COPY(STACK(base, 2), STACK(base, 3));
				LOAD_FIXNUM(0, STACK(base, 4));
				LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
				struct_ref(STACK(base, 3));
				COPY(STACK(base, 3), STACK(base, 0));
				LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 1));	/* PATTERN */
				struct_typep(STACK(base, 0));
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 184))	/* DIRECTORY */
				{
					LOAD_GLOBFUN(&Cpattern_p, STACK(base, 0));
					COPY(STACK(base, 2), STACK(base, 1));
					LOAD_FIXNUM(2, STACK(base, 2));
					LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 3));	/* PATHNAME */
					struct_ref(STACK(base, 1));
					Fsome(STACK(base, 0), 2);
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 185))	/* NAME */
					{
						COPY(STACK(base, 2), STACK(base, 3));
						LOAD_FIXNUM(3, STACK(base, 4));
						LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
						struct_ref(STACK(base, 3));
						COPY(STACK(base, 3), STACK(base, 0));
						LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 1));	/* PATTERN */
						struct_typep(STACK(base, 0));
					}
					else
					{
						if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 79))	/* TYPE */
						{
							COPY(STACK(base, 2), STACK(base, 3));
							LOAD_FIXNUM(4, STACK(base, 4));
							LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
							struct_ref(STACK(base, 3));
							COPY(STACK(base, 3), STACK(base, 0));
							LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 1));	/* PATTERN */
							struct_typep(STACK(base, 0));
						}
						else
						{
							if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 186))	/* VERSION */
							{
								COPY(STACK(base, 2), STACK(base, 3));
								LOAD_FIXNUM(5, STACK(base, 4));
								LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
								struct_ref(STACK(base, 3));
								if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 202))	/* WILD */
								{
									LOAD_T(STACK(base, 0));
								}
								else
								{
									LOAD_NIL(STACK(base, 0));
								}
							}
							else
							{
								LOAD_SMSTR((CL_FORM *)&KFwild_pathname_p[2], STACK(base, 0));	/* ecase: the value ~a is not a legal value */
								Ferror(STACK(base, 0), 2);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 3));
		LOAD_SYMBOL(SYMBOL(Slisp, 182), STACK(base, 4));	/* HOST */
		Fwild_pathname_p(STACK(base, 3), 2);
		if(CL_TRUEP(STACK(base, 3)))
		{
			COPY(STACK(base, 3), STACK(base, 0));
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 183), STACK(base, 5));	/* DEVICE */
			Fwild_pathname_p(STACK(base, 4), 2);
			if(CL_TRUEP(STACK(base, 4)))
			{
				COPY(STACK(base, 4), STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 2), STACK(base, 5));
				LOAD_SYMBOL(SYMBOL(Slisp, 184), STACK(base, 6));	/* DIRECTORY */
				Fwild_pathname_p(STACK(base, 5), 2);
				if(CL_TRUEP(STACK(base, 5)))
				{
					COPY(STACK(base, 5), STACK(base, 0));
				}
				else
				{
					COPY(STACK(base, 2), STACK(base, 6));
					LOAD_SYMBOL(SYMBOL(Slisp, 185), STACK(base, 7));	/* NAME */
					Fwild_pathname_p(STACK(base, 6), 2);
					if(CL_TRUEP(STACK(base, 6)))
					{
						COPY(STACK(base, 6), STACK(base, 0));
					}
					else
					{
						COPY(STACK(base, 2), STACK(base, 7));
						LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 8));	/* TYPE */
						Fwild_pathname_p(STACK(base, 7), 2);
						if(CL_TRUEP(STACK(base, 7)))
						{
							COPY(STACK(base, 7), STACK(base, 0));
						}
						else
						{
							COPY(STACK(base, 2), STACK(base, 0));
							LOAD_SYMBOL(SYMBOL(Slisp, 186), STACK(base, 1));	/* VERSION */
							Fwild_pathname_p(STACK(base, 0), 2);
						}
					}
				}
			}
		}
	}
}
