#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cbq_simplify = {bq_simplify, 1};

void bq_simplify(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(EQ(STACK(base, 1), SYMVAL(Slisp, 372)))	/* *BQ-QUOTE* */
		{
			COPY(STACK(base, 0), STACK(base, 1));
		}
		else
		{
			LOAD_GLOBFUN(&Cbq_simplify, STACK(base, 1));
			COPY(STACK(base, 0), STACK(base, 2));
			maptree(STACK(base, 1));
			mv_count = 1;
		}
		COPY(STACK(base, 1), STACK(base, 2));
		Fcar(STACK(base, 2));
		if(EQ(STACK(base, 2), SYMVAL(Slisp, 368)))	/* *BQ-APPEND* */
		{
			COPY(STACK(base, 1), STACK(base, 2));
			Fcdr(STACK(base, 2));
			Freverse(STACK(base, 2));
			LOAD_NIL(STACK(base, 3));
			M83_1:;
			if(CL_TRUEP(STACK(base, 2)))
			{
			}
			else
			{
				COPY(STACK(base, 3), STACK(base, 0));
				goto RETURN92;
			}
			COPY(STACK(base, 2), STACK(base, 4));
			Fcdr(STACK(base, 4));
			COPY(STACK(base, 2), STACK(base, 5));
			Fcar(STACK(base, 5));
			if(CL_ATOMP(STACK(base, 5)))
			{
				COPY(SYMVAL(Slisp, 368), STACK(base, 5));	/* *BQ-APPEND* */
				COPY(STACK(base, 2), STACK(base, 6));
				Fcar(STACK(base, 6));
				COPY(STACK(base, 3), STACK(base, 7));
				bq_attach_append(STACK(base, 5));
				COPY(STACK(base, 5), STACK(base, 3));
			}
			else
			{
				COPY(STACK(base, 2), STACK(base, 5));
				Fcar(STACK(base, 5));
				Fcar(STACK(base, 5));
				if(EQ(STACK(base, 5), SYMVAL(Slisp, 367)))	/* *BQ-LIST* */
				{
					LOAD_GLOBFUN(&Cbq_splicing_frob, STACK(base, 5));
					COPY(STACK(base, 2), STACK(base, 6));
					Fcar(STACK(base, 6));
					Fcdr(STACK(base, 6));
					Fnotany(STACK(base, 5), 2);
					bool_result = CL_TRUEP(STACK(base, 5));
				}
				else
				{
					bool_result = FALSE;
				}
				if(bool_result)
				{
					COPY(STACK(base, 2), STACK(base, 5));
					Fcar(STACK(base, 5));
					Fcdr(STACK(base, 5));
					COPY(STACK(base, 3), STACK(base, 6));
					bq_attach_conses(STACK(base, 5));
					COPY(STACK(base, 5), STACK(base, 3));
				}
				else
				{
					COPY(STACK(base, 2), STACK(base, 5));
					Fcar(STACK(base, 5));
					Fcar(STACK(base, 5));
					if(EQ(STACK(base, 5), SYMVAL(Slisp, 369)))	/* *BQ-LIST** */
					{
						LOAD_GLOBFUN(&Cbq_splicing_frob, STACK(base, 5));
						COPY(STACK(base, 2), STACK(base, 6));
						Fcar(STACK(base, 6));
						Fcdr(STACK(base, 6));
						Fnotany(STACK(base, 5), 2);
						bool_result = CL_TRUEP(STACK(base, 5));
					}
					else
					{
						bool_result = FALSE;
					}
					if(bool_result)
					{
						COPY(STACK(base, 2), STACK(base, 5));
						Fcar(STACK(base, 5));
						Fcdr(STACK(base, 5));
						Freverse(STACK(base, 5));
						Fcdr(STACK(base, 5));
						Freverse(STACK(base, 5));
						COPY(SYMVAL(Slisp, 368), STACK(base, 6));	/* *BQ-APPEND* */
						COPY(STACK(base, 2), STACK(base, 7));
						Fcar(STACK(base, 7));
						Flast(STACK(base, 7), 1);
						Fcar(STACK(base, 7));
						COPY(STACK(base, 3), STACK(base, 8));
						bq_attach_append(STACK(base, 6));
						bq_attach_conses(STACK(base, 5));
						COPY(STACK(base, 5), STACK(base, 3));
					}
					else
					{
						COPY(STACK(base, 2), STACK(base, 5));
						Fcar(STACK(base, 5));
						Fcar(STACK(base, 5));
						if(EQ(STACK(base, 5), SYMVAL(Slisp, 372)))	/* *BQ-QUOTE* */
						{
							COPY(STACK(base, 2), STACK(base, 5));
							Fcar(STACK(base, 5));
							Fcdr(STACK(base, 5));
							Fcar(STACK(base, 5));
							if(CL_CONSP(STACK(base, 5)))
							{
								COPY(STACK(base, 2), STACK(base, 5));
								Fcar(STACK(base, 5));
								Fcdr(STACK(base, 5));
								Fcar(STACK(base, 5));
								if(CL_CONSP(STACK(base, 5)))
								{
									COPY(STACK(base, 5), STACK(base, 6));
									Fcar(STACK(base, 6));
									if(EQ(STACK(base, 6), SYMVAL(Slisp, 364)))	/* *COMMA* */
									{
										LOAD_T(STACK(base, 6));
									}
									else
									{
										LOAD_NIL(STACK(base, 6));
									}
									if(CL_TRUEP(STACK(base, 6)))
									{
										bool_result = TRUE;
									}
									else
									{
										COPY(STACK(base, 5), STACK(base, 7));
										Fcar(STACK(base, 7));
										if(EQ(STACK(base, 7), SYMVAL(Slisp, 365)))	/* *COMMA-ATSIGN* */
										{
											LOAD_T(STACK(base, 7));
										}
										else
										{
											LOAD_NIL(STACK(base, 7));
										}
										if(CL_TRUEP(STACK(base, 7)))
										{
											bool_result = TRUE;
										}
										else
										{
											COPY(STACK(base, 5), STACK(base, 8));
											Fcar(STACK(base, 8));
											bool_result = EQ(STACK(base, 8), SYMVAL(Slisp, 366));	/* *COMMA-DOT* */
										}
									}
								}
								else
								{
									bool_result = FALSE;
								}
								if(bool_result)
								{
									bool_result = FALSE;
								}
								else
								{
									COPY(STACK(base, 2), STACK(base, 5));
									COPY(STACK(base, 5), STACK(base, 6));
									Fcar(STACK(base, 6));
									Fcdr(STACK(base, 6));
									Fcdr(STACK(base, 6));
									bool_result = NOT(CL_TRUEP(STACK(base, 6)));
								}
							}
							else
							{
								bool_result = FALSE;
							}
						}
						else
						{
							bool_result = FALSE;
						}
						if(bool_result)
						{
							COPY(SYMVAL(Slisp, 372), STACK(base, 5));	/* *BQ-QUOTE* */
							COPY(STACK(base, 2), STACK(base, 6));
							Fcar(STACK(base, 6));
							Fcdr(STACK(base, 6));
							Fcar(STACK(base, 6));
							Fcar(STACK(base, 6));
							Flist(STACK(base, 5), 2);
							Flist(STACK(base, 5), 1);
							COPY(STACK(base, 3), STACK(base, 6));
							bq_attach_conses(STACK(base, 5));
							COPY(STACK(base, 5), STACK(base, 3));
						}
						else
						{
							COPY(STACK(base, 2), STACK(base, 5));
							Fcar(STACK(base, 5));
							Fcar(STACK(base, 5));
							if(EQ(STACK(base, 5), SYMVAL(Slisp, 371)))	/* *BQ-CLOBBERABLE* */
							{
								COPY(SYMVAL(Slisp, 370), STACK(base, 5));	/* *BQ-NCONC* */
								COPY(STACK(base, 2), STACK(base, 6));
								Fcar(STACK(base, 6));
								Fcdr(STACK(base, 6));
								Fcar(STACK(base, 6));
								COPY(STACK(base, 3), STACK(base, 7));
								bq_attach_append(STACK(base, 5));
								COPY(STACK(base, 5), STACK(base, 3));
							}
							else
							{
								COPY(SYMVAL(Slisp, 368), STACK(base, 5));	/* *BQ-APPEND* */
								COPY(STACK(base, 2), STACK(base, 6));
								Fcar(STACK(base, 6));
								COPY(STACK(base, 3), STACK(base, 7));
								bq_attach_append(STACK(base, 5));
								COPY(STACK(base, 5), STACK(base, 3));
							}
						}
					}
				}
			}
			COPY(STACK(base, 4), STACK(base, 2));
			goto M83_1;
			RETURN92:;
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 0));
		}
	}
}
