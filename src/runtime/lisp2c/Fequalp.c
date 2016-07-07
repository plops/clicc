#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFequalp = {Fequalp, 2};

void Fequalp(CL_FORM *base)
{
	M234_1:;
	if(CL_CHARP(STACK(base, 0)))
	{
		if(CL_CHARP(STACK(base, 1)))
		{
			Fchar_equal(STACK(base, 0), 2);
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
	else
	{
		if(CL_NUMBERP(STACK(base, 0)))
		{
			if(CL_NUMBERP(STACK(base, 1)))
			{
				Fnumeql(STACK(base, 0), 2);
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
		else
		{
			if(CL_CONSP(STACK(base, 0)))
			{
				if(CL_CONSP(STACK(base, 1)))
				{
					COPY(STACK(base, 0), STACK(base, 2));
					Fcar(STACK(base, 2));
					COPY(STACK(base, 1), STACK(base, 3));
					Fcar(STACK(base, 3));
					Fequalp(STACK(base, 2));
					if(CL_TRUEP(STACK(base, 2)))
					{
						COPY(STACK(base, 0), STACK(base, 2));
						Fcdr(STACK(base, 2));
						COPY(STACK(base, 1), STACK(base, 3));
						Fcdr(STACK(base, 3));
						COPY(STACK(base, 2), STACK(base, 0));
						COPY(STACK(base, 3), STACK(base, 1));
						goto M234_1;
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
				if(CL_VECTOR_P(STACK(base, 0)))
				{
					if(CL_VECTOR_P(STACK(base, 1)))
					{
						COPY(STACK(base, 0), STACK(base, 2));
						Flength(STACK(base, 2));
						COPY(STACK(base, 1), STACK(base, 3));
						Flength(STACK(base, 3));
						if(EQ(STACK(base, 2), STACK(base, 3)) || CL_FLOATP(STACK(base, 2)) && GET_FLOAT(STACK(base, 2)) == GET_FLOAT(STACK(base, 3)))
						{
							COPY(STACK(base, 0), STACK(base, 2));
							Flength(STACK(base, 2));
							LOAD_FIXNUM(0, STACK(base, 3));
							M235_1:;
							COPY(STACK(base, 3), STACK(base, 4));
							COPY(STACK(base, 2), STACK(base, 5));
							Fge(STACK(base, 4), 2);
							if(CL_TRUEP(STACK(base, 4)))
							{
								LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
								goto RETURN275;
							}
							COPY(STACK(base, 0), STACK(base, 4));
							COPY(STACK(base, 3), STACK(base, 5));
							vref(STACK(base, 4));
							COPY(STACK(base, 1), STACK(base, 5));
							COPY(STACK(base, 3), STACK(base, 6));
							vref(STACK(base, 5));
							Fequalp(STACK(base, 4));
							if(CL_TRUEP(STACK(base, 4)))
							{
							}
							else
							{
								LOAD_NIL(STACK(base, 0));
								goto RETURN275;
							}
							F1plus(STACK(base, 3));
							goto M235_1;
							RETURN275:;
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
					if(CL_ARRAY_P(STACK(base, 0)))
					{
						if(CL_ARRAY_P(STACK(base, 1)))
						{
							COPY(STACK(base, 0), STACK(base, 2));
							Farray_rank(STACK(base, 2));
							COPY(STACK(base, 1), STACK(base, 3));
							Farray_rank(STACK(base, 3));
							if(EQ(STACK(base, 2), STACK(base, 3)) || CL_FLOATP(STACK(base, 2)) && GET_FLOAT(STACK(base, 2)) == GET_FLOAT(STACK(base, 3)))
							{
								COPY(STACK(base, 0), STACK(base, 2));
								Farray_rank(STACK(base, 2));
								LOAD_FIXNUM(0, STACK(base, 3));
								M236_1:;
								COPY(STACK(base, 3), STACK(base, 4));
								COPY(STACK(base, 2), STACK(base, 5));
								Fge(STACK(base, 4), 2);
								if(CL_TRUEP(STACK(base, 4)))
								{
									bool_result = TRUE;
									goto RETURN276;
								}
								COPY(STACK(base, 0), STACK(base, 4));
								COPY(STACK(base, 3), STACK(base, 5));
								Farray_dimension(STACK(base, 4));
								COPY(STACK(base, 1), STACK(base, 5));
								COPY(STACK(base, 3), STACK(base, 6));
								Farray_dimension(STACK(base, 5));
								if(EQ(STACK(base, 4), STACK(base, 5)) || CL_FLOATP(STACK(base, 4)) && GET_FLOAT(STACK(base, 4)) == GET_FLOAT(STACK(base, 5)))
								{
								}
								else
								{
									bool_result = FALSE;
									goto RETURN276;
								}
								F1plus(STACK(base, 3));
								goto M236_1;
								RETURN276:;
								if(bool_result)
								{
									COPY(STACK(base, 0), STACK(base, 2));
									Farray_total_size(STACK(base, 2));
									LOAD_FIXNUM(0, STACK(base, 3));
									M237_1:;
									COPY(STACK(base, 3), STACK(base, 4));
									COPY(STACK(base, 2), STACK(base, 5));
									Fge(STACK(base, 4), 2);
									if(CL_TRUEP(STACK(base, 4)))
									{
										LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
										goto RETURN277;
									}
									COPY(STACK(base, 0), STACK(base, 4));
									COPY(STACK(base, 3), STACK(base, 5));
									Frow_major_aref(STACK(base, 4));
									COPY(STACK(base, 1), STACK(base, 5));
									COPY(STACK(base, 3), STACK(base, 6));
									Frow_major_aref(STACK(base, 5));
									Fequalp(STACK(base, 4));
									if(CL_TRUEP(STACK(base, 4)))
									{
									}
									else
									{
										LOAD_NIL(STACK(base, 0));
										goto RETURN277;
									}
									F1plus(STACK(base, 3));
									goto M237_1;
									RETURN277:;
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
						COPY(STACK(base, 0), STACK(base, 2));
						structp(STACK(base, 2));
						if(CL_TRUEP(STACK(base, 2)))
						{
							COPY(STACK(base, 1), STACK(base, 2));
							structp(STACK(base, 2));
							if(CL_TRUEP(STACK(base, 2)))
							{
								COPY(STACK(base, 0), STACK(base, 2));
								struct_type(STACK(base, 2));
								COPY(STACK(base, 1), STACK(base, 3));
								struct_type(STACK(base, 3));
								if(EQ(STACK(base, 2), STACK(base, 3)))
								{
									COPY(STACK(base, 0), STACK(base, 2));
									struct_type(STACK(base, 2));
									COPY(STACK(base, 0), STACK(base, 3));
									struct_size(STACK(base, 3));
									LOAD_FIXNUM(0, STACK(base, 4));
									M238_1:;
									COPY(STACK(base, 4), STACK(base, 5));
									COPY(STACK(base, 3), STACK(base, 6));
									Fge(STACK(base, 5), 2);
									if(CL_TRUEP(STACK(base, 5)))
									{
										LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
										goto RETURN278;
									}
									COPY(STACK(base, 0), STACK(base, 5));
									COPY(STACK(base, 4), STACK(base, 6));
									COPY(STACK(base, 2), STACK(base, 7));
									struct_ref(STACK(base, 5));
									COPY(STACK(base, 1), STACK(base, 6));
									COPY(STACK(base, 4), STACK(base, 7));
									COPY(STACK(base, 2), STACK(base, 8));
									struct_ref(STACK(base, 6));
									Fequalp(STACK(base, 5));
									if(CL_TRUEP(STACK(base, 5)))
									{
									}
									else
									{
										LOAD_NIL(STACK(base, 0));
										goto RETURN278;
									}
									F1plus(STACK(base, 4));
									goto M238_1;
									RETURN278:;
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
							if(EQ(STACK(base, 0), STACK(base, 1)))
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
			}
		}
	}
	goto RETURN274;
	RETURN274:;
}
