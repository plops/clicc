#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void internal_sxhash(CL_FORM *base)
{
	M80_1:;
	if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
	{
		Fsymbol_name(STACK(base, 0));
		sxhash_simple_string(STACK(base, 0));
	}
	else
	{
		if(CL_LISTP(STACK(base, 0)))
		{
			COPY(STACK(base, 1), STACK(base, 2));
			LOAD_FIXNUM(3, STACK(base, 3));
			Fnumeql(STACK(base, 2), 2);
			if(CL_TRUEP(STACK(base, 2)))
			{
				LOAD_FIXNUM(0, STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 2));
				LOAD_FIXNUM(0, STACK(base, 3));
				LOAD_FIXNUM(2, STACK(base, 4));
				M81_1:;
				if(CL_ATOMP(STACK(base, 2)))
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
					COPY(STACK(base, 3), STACK(base, 6));
					LOAD_FIXNUM(7, STACK(base, 7));
					Fnumeql(STACK(base, 6), 2);
					bool_result = CL_TRUEP(STACK(base, 6));
				}
				if(bool_result)
				{
					COPY(STACK(base, 4), STACK(base, 0));
					goto RETURN90;
				}
				COPY(STACK(base, 4), STACK(base, 5));
				COPY(STACK(base, 2), STACK(base, 6));
				Fcar(STACK(base, 6));
				COPY(STACK(base, 1), STACK(base, 7));
				F1plus(STACK(base, 7));
				internal_sxhash(STACK(base, 6));
				mv_count = 1;
				combine_hash(STACK(base, 5));
				COPY(STACK(base, 2), STACK(base, 5));
				Fcdr(STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				F1plus(STACK(base, 6));
				COPY(STACK(base, 6), STACK(base, 3));
				COPY(STACK(base, 5), STACK(base, 2));
				goto M81_1;
				RETURN90:;
			}
		}
		else
		{
			if(CL_SMAR_P(STACK(base, 0)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
				check_array_internal(STACK(base, 2));
				bool_result = CL_TRUEP(STACK(base, 2));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				sxhash_simple_string(STACK(base, 0));
			}
			else
			{
				if(CL_FIXNUMP(STACK(base, 0)))
				{
				}
				else
				{
					if(CL_FLOATP(STACK(base, 0)))
					{
						LOAD_FIXNUM(1, STACK(base, 1));
						truncate_internal(STACK(base, 0));
					}
					else
					{
						if(CL_CHARP(STACK(base, 0)))
						{
							Fchar_code(STACK(base, 0));
						}
						else
						{
							COPY(STACK(base, 0), STACK(base, 2));
							structp(STACK(base, 2));
							if(CL_TRUEP(STACK(base, 2)))
							{
								COPY(STACK(base, 0), STACK(base, 2));
								struct_type(STACK(base, 2));
								COPY(STACK(base, 2), STACK(base, 0));
								goto M80_1;
							}
							else
							{
								if(CL_ARRAY_P(STACK(base, 0)))
								{
									COPY(STACK(base, 0), STACK(base, 2));
									LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
									LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
									check_array_internal(STACK(base, 2));
									bool_result = CL_TRUEP(STACK(base, 2));
								}
								else
								{
									bool_result = FALSE;
								}
								if(bool_result)
								{
									sxhash_string(STACK(base, 0));
								}
								else
								{
									if(CL_ARRAY_P(STACK(base, 0)))
									{
										Farray_rank(STACK(base, 0));
									}
									else
									{
										LOAD_FIXNUM(42, STACK(base, 0));
									}
								}
							}
						}
					}
				}
			}
		}
	}
	goto RETURN89;
	RETURN89:;
}
