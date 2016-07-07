#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFtypep)
{
	MAKE_STRING(32, "(TYPEP ~S ~S) is not implemented"),	/* 0 */
};

void Ftypep(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Ssys, 1))	/* T */
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		if(CL_TRUEP(STACK(base, 1)))
		{
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 23))	/* FIXNUM */
			{
				if(CL_FIXNUMP(STACK(base, 0)))
				{
					LOAD_FIXNUM((-2147483647) - 1, STACK(base, 2));
					COPY(STACK(base, 0), STACK(base, 3));
					LOAD_FIXNUM(2147483647, STACK(base, 4));
					Fle(STACK(base, 2), 3);
					COPY(STACK(base, 2), STACK(base, 0));
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
				}
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 24))	/* FLOAT */
				{
					LOAD_T(STACK(base, 2));
				}
				else
				{
					LOAD_NIL(STACK(base, 2));
				}
				if(CL_TRUEP(STACK(base, 2)))
				{
					bool_result = TRUE;
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 37))	/* SHORT-FLOAT */
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
						if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 42))	/* SINGLE-FLOAT */
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
							if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 22))	/* DOUBLE-FLOAT */
							{
								LOAD_T(STACK(base, 5));
							}
							else
							{
								LOAD_NIL(STACK(base, 5));
							}
							if(CL_TRUEP(STACK(base, 5)))
							{
								bool_result = TRUE;
							}
							else
							{
								bool_result = CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 29);	/* LONG-FLOAT */
							}
						}
					}
				}
				if(bool_result)
				{
					if(CL_FLOATP(STACK(base, 0)))
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
					if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 18))	/* CHARACTER */
					{
						LOAD_T(STACK(base, 2));
					}
					else
					{
						LOAD_NIL(STACK(base, 2));
					}
					if(CL_TRUEP(STACK(base, 2)))
					{
						bool_result = TRUE;
					}
					else
					{
						bool_result = CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 43);	/* STANDARD-CHAR */
					}
					if(bool_result)
					{
						if(CL_CHARP(STACK(base, 0)))
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
						if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 28))	/* LIST */
						{
							if(CL_LISTP(STACK(base, 0)))
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
							if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 31))	/* NULL */
							{
								if(NOT(CL_TRUEP(STACK(base, 0))))
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
								if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 21))	/* CONS */
								{
									if(CL_CONSP(STACK(base, 0)))
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
									LOAD_SMSTR((CL_FORM *)&KFtypep[0], STACK(base, 2));	/* (TYPEP ~S ~S) is not implemented */
									COPY(STACK(base, 0), STACK(base, 3));
									COPY(STACK(base, 1), STACK(base, 4));
									Ferror(STACK(base, 2), 3);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
}
