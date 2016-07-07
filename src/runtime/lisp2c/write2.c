#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kwrite2)
{
	MAKE_STRING(2, "#\\"),	/* 0 */
	MAKE_STRING(2, "||"),	/* 2 */
	MAKE_STRING(5, "-.+^_"),	/* 4 */
	MAKE_STRING(16, "-.eEfFdDlLsS+/^_"),	/* 6 */
	MAKE_STRING(2, "#:"),	/* 8 */
	MAKE_STRING(3, "..."),	/* 10 */
	MAKE_STRING(3, " . "),	/* 12 */
	MAKE_STRING(4, " ..."),	/* 14 */
	MAKE_STRING(3, "#()"),	/* 16 */
	MAKE_STRING(2, "#("),	/* 18 */
	MAKE_STRING(3, "..."),	/* 20 */
	MAKE_STRING(4, " ..."),	/* 22 */
	MAKE_STRING(9, "#<vector>"),	/* 24 */
	MAKE_STRING(8, "#<array>"),	/* 26 */
	MAKE_STRING(11, "#<function>"),	/* 28 */
	MAKE_STRING(12, "#<readtable>"),	/* 30 */
	MAKE_STRING(9, "#<stream>"),	/* 32 */
	MAKE_STRING(11, "#<Package \""),	/* 34 */
	MAKE_STRING(2, "\">"),	/* 36 */
	MAKE_STRING(3, "#P\""),	/* 38 */
	MAKE_STRING(14, "#<INSTANCE of "),	/* 40 */
	MAKE_STRING(1, ">"),	/* 42 */
	MAKE_STRING(3, "#S("),	/* 44 */
	MAKE_STRING(4, " ..."),	/* 46 */
	MAKE_STRING(9, " #<slot> "),	/* 48 */
	MAKE_STRING(9, "<C(char):"),	/* 50 */
	MAKE_STRING(1, ">"),	/* 52 */
	MAKE_STRING(12, "<C(integer):"),	/* 54 */
	MAKE_STRING(1, ">"),	/* 56 */
	MAKE_STRING(10, "<C(float):"),	/* 58 */
	MAKE_STRING(1, ">"),	/* 60 */
	MAKE_STRING(11, "<C(struct)>"),	/* 62 */
	MAKE_STRING(10, "#<Unbound>"),	/* 64 */
	MAKE_STRING(24, "#<!!! unknown type !!!!>"),	/* 66 */
};

static void Z95_write_symbol_name(CL_FORM *base, CL_FORM *display[]);

void write2(CL_FORM *base)
{
	CL_FORM *display[1];
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		print_integer(STACK(base, 2));
		mv_count = 1;
	}
	else
	{
		if(CL_CHARP(STACK(base, 0)))
		{
			if(CL_TRUEP(SYMVAL(Slisp, 403)))	/* *PRINT-ESCAPE* */
			{
				COPY(STACK(base, 0), STACK(base, 2));
				Fchar_name(STACK(base, 2));
				LOAD_SMSTR((CL_FORM *)&Kwrite2[0], STACK(base, 3));	/* #\ */
				COPY(STACK(base, 1), STACK(base, 4));
				Fwrite_string(STACK(base, 3), 2);
				if(CL_TRUEP(STACK(base, 2)))
				{
					COPY(STACK(base, 2), STACK(base, 3));
					COPY(STACK(base, 1), STACK(base, 4));
					Fwrite_string(STACK(base, 3), 2);
				}
				else
				{
					COPY(STACK(base, 0), STACK(base, 3));
					COPY(STACK(base, 1), STACK(base, 4));
					Fwrite_char(STACK(base, 3), 2);
					mv_count = 1;
				}
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 2));
				COPY(STACK(base, 1), STACK(base, 3));
				Fwrite_char(STACK(base, 2), 2);
				mv_count = 1;
			}
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				Fsymbol_name(STACK(base, 2));
				if(CL_TRUEP(SYMVAL(Slisp, 403)))	/* *PRINT-ESCAPE* */
				{
					COPY(STACK(base, 0), STACK(base, 3));
					Fkeywordp(STACK(base, 3));
					if(CL_TRUEP(STACK(base, 3)))
					{
						LOAD_CHAR(':', STACK(base, 3));
						COPY(STACK(base, 1), STACK(base, 4));
						Fwrite_char(STACK(base, 3), 2);
						mv_count = 1;
					}
					else
					{
						COPY(STACK(base, 0), STACK(base, 3));
						Fsymbol_package(STACK(base, 3));
						if(CL_TRUEP(STACK(base, 3)))
						{
							COPY(STACK(base, 2), STACK(base, 3));
							Ffind_symbol(STACK(base, 3), 1);
							COPY(&mv_buf[0], STACK(base, 4));
							{
								int nargs;
								nargs = 2;
								mv_count = 1;
								switch(nargs)
								{
									case 0:
									LOAD_NIL(STACK(base, 3));
									case 1:
									LOAD_NIL(STACK(base, 4));
									nargs = 2;
								}
								Flist(STACK(base, 5), nargs - 2);
								if(NOT(CL_TRUEP(STACK(base, 4))))
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
									bool_result = NOT(EQ(STACK(base, 0), STACK(base, 3)));
								}
								if(bool_result)
								{
									COPY(STACK(base, 0), STACK(base, 6));
									Fsymbol_package(STACK(base, 6));
									COPY(STACK(base, 6), STACK(base, 7));
									COPY(STACK(base, 6), STACK(base, 8));
									LOAD_FIXNUM(0, STACK(base, 9));
									LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 10));	/* PACKAGE */
									struct_ref(STACK(base, 8));
									COPY(STACK(base, 8), STACK(base, 7));
									display[0] = STACK(base, 0);
									Z95_write_symbol_name(STACK(base, 7), display);
									mv_count = 1;
									LOAD_CHAR(':', STACK(base, 7));
									COPY(STACK(base, 1), STACK(base, 8));
									Fwrite_char(STACK(base, 7), 2);
									mv_count = 1;
									COPY(STACK(base, 2), STACK(base, 7));
									COPY(STACK(base, 6), STACK(base, 8));
									Ffind_symbol(STACK(base, 7), 2);
									COPY(&mv_buf[0], STACK(base, 8));
									{
										int nargs;
										nargs = 2;
										mv_count = 1;
										switch(nargs)
										{
											case 0:
											LOAD_NIL(STACK(base, 7));
											case 1:
											LOAD_NIL(STACK(base, 8));
											nargs = 2;
										}
										Flist(STACK(base, 9), nargs - 2);
										if(CL_SYMBOLP(STACK(base, 8)) && GET_SYMBOL(STACK(base, 8)) == SYMBOL(Slisp, 305))	/* INTERNAL */
										{
											LOAD_CHAR(':', STACK(base, 10));
											COPY(STACK(base, 1), STACK(base, 11));
											Fwrite_char(STACK(base, 10), 2);
											mv_count = 1;
										}
									}
								}
							}
						}
						else
						{
							if(CL_TRUEP(SYMVAL(Slisp, 404)))	/* *PRINT-GENSYM* */
							{
								LOAD_SMSTR((CL_FORM *)&Kwrite2[8], STACK(base, 3));	/* #: */
								COPY(STACK(base, 1), STACK(base, 4));
								Fwrite_string(STACK(base, 3), 2);
							}
						}
					}
					COPY(STACK(base, 2), STACK(base, 3));
					display[0] = STACK(base, 0);
					Z95_write_symbol_name(STACK(base, 3), display);
					mv_count = 1;
				}
				else
				{
					if(CL_SYMBOLP(SYMVAL(Slisp, 401)) && GET_SYMBOL(SYMVAL(Slisp, 401)) == SYMBOL(Slisp, 421))	/* *PRINT-CASE* UPCASE */
					{
						COPY(STACK(base, 2), STACK(base, 3));
						COPY(STACK(base, 1), STACK(base, 4));
						Fwrite_string(STACK(base, 3), 2);
					}
					else
					{
						if(CL_SYMBOLP(SYMVAL(Slisp, 401)) && GET_SYMBOL(SYMVAL(Slisp, 401)) == SYMBOL(Slisp, 429))	/* *PRINT-CASE* DOWNCASE */
						{
							COPY(STACK(base, 2), STACK(base, 3));
							Fstring_downcase(STACK(base, 3), 1);
							COPY(STACK(base, 1), STACK(base, 4));
							Fwrite_string(STACK(base, 3), 2);
						}
						else
						{
							if(CL_SYMBOLP(SYMVAL(Slisp, 401)) && GET_SYMBOL(SYMVAL(Slisp, 401)) == SYMBOL(Slisp, 430))	/* *PRINT-CASE* CAPITALIZE */
							{
								COPY(STACK(base, 2), STACK(base, 3));
								Fstring_capitalize(STACK(base, 3), 1);
								COPY(STACK(base, 1), STACK(base, 4));
								Fwrite_string(STACK(base, 3), 2);
							}
						}
					}
				}
			}
			else
			{
				if(CL_STRING_P(STACK(base, 0)))
				{
					if(CL_TRUEP(SYMVAL(Slisp, 403)))	/* *PRINT-ESCAPE* */
					{
						LOAD_CHAR('\"', STACK(base, 2));
						COPY(STACK(base, 1), STACK(base, 3));
						Fwrite_char(STACK(base, 2), 2);
						mv_count = 1;
						COPY(STACK(base, 0), STACK(base, 2));
						Flength(STACK(base, 2));
						LOAD_FIXNUM(0, STACK(base, 3));
						M110_1:;
						COPY(STACK(base, 3), STACK(base, 4));
						COPY(STACK(base, 2), STACK(base, 5));
						Fge(STACK(base, 4), 2);
						if(CL_TRUEP(STACK(base, 4)))
						{
							goto RETURN122;
						}
						COPY(STACK(base, 0), STACK(base, 4));
						COPY(STACK(base, 3), STACK(base, 5));
						Fchar(STACK(base, 4));
						if(CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '\\')
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
							bool_result = CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '\"';
						}
						if(bool_result)
						{
							LOAD_CHAR('\\', STACK(base, 5));
							COPY(STACK(base, 1), STACK(base, 6));
							Fwrite_char(STACK(base, 5), 2);
							mv_count = 1;
						}
						COPY(STACK(base, 4), STACK(base, 5));
						COPY(STACK(base, 1), STACK(base, 6));
						Fwrite_char(STACK(base, 5), 2);
						mv_count = 1;
						COPY(STACK(base, 3), STACK(base, 4));
						F1plus(STACK(base, 4));
						COPY(STACK(base, 4), STACK(base, 3));
						goto M110_1;
						RETURN122:;
						LOAD_CHAR('\"', STACK(base, 2));
						COPY(STACK(base, 1), STACK(base, 3));
						Fwrite_char(STACK(base, 2), 2);
						mv_count = 1;
					}
					else
					{
						COPY(STACK(base, 0), STACK(base, 2));
						COPY(STACK(base, 1), STACK(base, 3));
						Fwrite_string(STACK(base, 2), 2);
					}
				}
				else
				{
					if(CL_CONSP(STACK(base, 0)))
					{
						if(CL_FIXNUMP(SYMVAL(Slisp, 406)) && GET_FIXNUM(SYMVAL(Slisp, 406)) == 0)	/* *PRINT-LEVEL* */
						{
							LOAD_CHAR('#', STACK(base, 2));
							COPY(STACK(base, 1), STACK(base, 3));
							Fwrite_char(STACK(base, 2), 2);
							mv_count = 1;
						}
						else
						{
							if(CL_TRUEP(SYMVAL(Slisp, 406)))	/* *PRINT-LEVEL* */
							{
								COPY(SYMVAL(Slisp, 406), STACK(base, 2));	/* *PRINT-LEVEL* */
								F1minus(STACK(base, 2));
							}
							else
							{
								COPY(SYMVAL(Slisp, 406), STACK(base, 2));	/* *PRINT-LEVEL* */
							}
							COPY(STACK(base, 2), STACK(base, 3));
							BIND_SPECIAL(SYMBOL(Slisp, 406), STACK(base, 3));	/* *PRINT-LEVEL* */
							LOAD_CHAR('(', STACK(base, 4));
							COPY(STACK(base, 1), STACK(base, 5));
							Fwrite_char(STACK(base, 4), 2);
							mv_count = 1;
							if(CL_FIXNUMP(SYMVAL(Slisp, 405)) && GET_FIXNUM(SYMVAL(Slisp, 405)) == 0)	/* *PRINT-LENGTH* */
							{
								LOAD_SMSTR((CL_FORM *)&Kwrite2[10], STACK(base, 4));	/* ... */
								COPY(STACK(base, 1), STACK(base, 5));
								Fwrite_string(STACK(base, 4), 2);
							}
							else
							{
								COPY(STACK(base, 0), STACK(base, 4));
								Fcar(STACK(base, 4));
								COPY(STACK(base, 1), STACK(base, 5));
								write2(STACK(base, 4));
								COPY(STACK(base, 0), STACK(base, 4));
								Fcdr(STACK(base, 4));
								LOAD_FIXNUM(1, STACK(base, 5));
								M111_1:;
								if(CL_ATOMP(STACK(base, 4)))
								{
									if(CL_TRUEP(STACK(base, 4)))
									{
										LOAD_SMSTR((CL_FORM *)&Kwrite2[12], STACK(base, 6));	/*  .  */
										COPY(STACK(base, 1), STACK(base, 7));
										Fwrite_string(STACK(base, 6), 2);
										COPY(STACK(base, 4), STACK(base, 6));
										COPY(STACK(base, 1), STACK(base, 7));
										write2(STACK(base, 6));
									}
									bool_result = TRUE;
								}
								else
								{
									bool_result = CL_TRUEP(SYMVAL(Slisp, 405));	/* *PRINT-LENGTH* */
									if(bool_result)
									{
										COPY(STACK(base, 5), STACK(base, 6));
										COPY(SYMVAL(Slisp, 405), STACK(base, 7));	/* *PRINT-LENGTH* */
										Fge(STACK(base, 6), 2);
										bool_result = CL_TRUEP(STACK(base, 6));
									}
									else
									{
										bool_result = FALSE;
									}
									if(bool_result)
									{
										LOAD_SMSTR((CL_FORM *)&Kwrite2[14], STACK(base, 6));	/*  ... */
										COPY(STACK(base, 1), STACK(base, 7));
										Fwrite_string(STACK(base, 6), 2);
										bool_result = TRUE;
									}
									else
									{
										bool_result = FALSE;
									}
								}
								if(bool_result)
								{
									goto RETURN123;
								}
								LOAD_CHAR(' ', STACK(base, 6));
								COPY(STACK(base, 1), STACK(base, 7));
								Fwrite_char(STACK(base, 6), 2);
								mv_count = 1;
								COPY(STACK(base, 4), STACK(base, 6));
								Fcar(STACK(base, 6));
								COPY(STACK(base, 1), STACK(base, 7));
								write2(STACK(base, 6));
								COPY(STACK(base, 4), STACK(base, 6));
								Fcdr(STACK(base, 6));
								COPY(STACK(base, 5), STACK(base, 7));
								F1plus(STACK(base, 7));
								COPY(STACK(base, 7), STACK(base, 5));
								COPY(STACK(base, 6), STACK(base, 4));
								goto M111_1;
								RETURN123:;
							}
							LOAD_CHAR(')', STACK(base, 4));
							COPY(STACK(base, 1), STACK(base, 5));
							Fwrite_char(STACK(base, 4), 2);
							mv_count = 1;
							RESTORE_SPECIAL;
						}
					}
					else
					{
						if(CL_VECTOR_P(STACK(base, 0)))
						{
							if(CL_TRUEP(SYMVAL(Slisp, 399)))	/* *PRINT-ARRAY* */
							{
								if(CL_FIXNUMP(SYMVAL(Slisp, 406)) && GET_FIXNUM(SYMVAL(Slisp, 406)) == 0)	/* *PRINT-LEVEL* */
								{
									LOAD_CHAR('#', STACK(base, 2));
									COPY(STACK(base, 1), STACK(base, 3));
									Fwrite_char(STACK(base, 2), 2);
									mv_count = 1;
								}
								else
								{
									COPY(STACK(base, 0), STACK(base, 2));
									Flength(STACK(base, 2));
									if(CL_FIXNUMP(STACK(base, 2)) && GET_FIXNUM(STACK(base, 2)) == 0)
									{
										LOAD_SMSTR((CL_FORM *)&Kwrite2[16], STACK(base, 2));	/* #() */
										COPY(STACK(base, 1), STACK(base, 3));
										Fwrite_string(STACK(base, 2), 2);
									}
									else
									{
										LOAD_SMSTR((CL_FORM *)&Kwrite2[18], STACK(base, 2));	/* #( */
										COPY(STACK(base, 1), STACK(base, 3));
										Fwrite_string(STACK(base, 2), 2);
										if(CL_FIXNUMP(SYMVAL(Slisp, 405)) && GET_FIXNUM(SYMVAL(Slisp, 405)) == 0)	/* *PRINT-LENGTH* */
										{
											LOAD_SMSTR((CL_FORM *)&Kwrite2[20], STACK(base, 2));	/* ... */
											COPY(STACK(base, 1), STACK(base, 3));
											Fwrite_string(STACK(base, 2), 2);
										}
										else
										{
											if(CL_TRUEP(SYMVAL(Slisp, 406)))	/* *PRINT-LEVEL* */
											{
												COPY(SYMVAL(Slisp, 406), STACK(base, 2));	/* *PRINT-LEVEL* */
												F1minus(STACK(base, 2));
											}
											else
											{
												COPY(SYMVAL(Slisp, 406), STACK(base, 2));	/* *PRINT-LEVEL* */
											}
											LOAD_FIXNUM(1, STACK(base, 3));
											COPY(STACK(base, 0), STACK(base, 4));
											Flength(STACK(base, 4));
											COPY(STACK(base, 2), STACK(base, 5));
											BIND_SPECIAL(SYMBOL(Slisp, 406), STACK(base, 5));	/* *PRINT-LEVEL* */
											COPY(STACK(base, 0), STACK(base, 6));
											LOAD_FIXNUM(0, STACK(base, 7));
											vref(STACK(base, 6));
											COPY(STACK(base, 1), STACK(base, 7));
											write2(STACK(base, 6));
											M112_1:;
											COPY(STACK(base, 3), STACK(base, 6));
											COPY(STACK(base, 4), STACK(base, 7));
											Fge(STACK(base, 6), 2);
											if(CL_TRUEP(STACK(base, 6)))
											{
												goto RETURN124;
											}
											bool_result = CL_TRUEP(SYMVAL(Slisp, 405));	/* *PRINT-LENGTH* */
											if(bool_result)
											{
												COPY(STACK(base, 3), STACK(base, 6));
												COPY(SYMVAL(Slisp, 405), STACK(base, 7));	/* *PRINT-LENGTH* */
												Fge(STACK(base, 6), 2);
												bool_result = CL_TRUEP(STACK(base, 6));
											}
											else
											{
												bool_result = FALSE;
											}
											if(bool_result)
											{
												LOAD_SMSTR((CL_FORM *)&Kwrite2[22], STACK(base, 6));	/*  ... */
												COPY(STACK(base, 1), STACK(base, 7));
												Fwrite_string(STACK(base, 6), 2);
												goto RETURN124;
											}
											LOAD_CHAR(' ', STACK(base, 6));
											COPY(STACK(base, 1), STACK(base, 7));
											Fwrite_char(STACK(base, 6), 2);
											mv_count = 1;
											COPY(STACK(base, 0), STACK(base, 6));
											COPY(STACK(base, 3), STACK(base, 7));
											vref(STACK(base, 6));
											COPY(STACK(base, 1), STACK(base, 7));
											write2(STACK(base, 6));
											COPY(STACK(base, 3), STACK(base, 6));
											F1plus(STACK(base, 6));
											COPY(STACK(base, 6), STACK(base, 3));
											goto M112_1;
											RETURN124:;
											RESTORE_SPECIAL;
										}
										LOAD_CHAR(')', STACK(base, 2));
										COPY(STACK(base, 1), STACK(base, 3));
										Fwrite_char(STACK(base, 2), 2);
										mv_count = 1;
									}
								}
							}
							else
							{
								LOAD_SMSTR((CL_FORM *)&Kwrite2[24], STACK(base, 2));	/* #<vector> */
								COPY(STACK(base, 1), STACK(base, 3));
								Fwrite_string(STACK(base, 2), 2);
							}
						}
						else
						{
							if(CL_ARRAY_P(STACK(base, 0)))
							{
								LOAD_SMSTR((CL_FORM *)&Kwrite2[26], STACK(base, 2));	/* #<array> */
								COPY(STACK(base, 1), STACK(base, 3));
								Fwrite_string(STACK(base, 2), 2);
							}
							else
							{
								if(CL_FLOATP(STACK(base, 0)))
								{
									COPY(STACK(base, 0), STACK(base, 2));
									COPY(STACK(base, 1), STACK(base, 3));
									print_float(STACK(base, 2));
								}
								else
								{
									if(CL_FUNCTION_P(STACK(base, 0)))
									{
										LOAD_SMSTR((CL_FORM *)&Kwrite2[28], STACK(base, 2));	/* #<function> */
										COPY(STACK(base, 1), STACK(base, 3));
										Fwrite_string(STACK(base, 2), 2);
									}
									else
									{
										COPY(STACK(base, 0), STACK(base, 2));
										LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 3));	/* READTABLE */
										struct_typep(STACK(base, 2));
										if(CL_TRUEP(STACK(base, 2)))
										{
											LOAD_SMSTR((CL_FORM *)&Kwrite2[30], STACK(base, 2));	/* #<readtable> */
											COPY(STACK(base, 1), STACK(base, 3));
											Fwrite_string(STACK(base, 2), 2);
										}
										else
										{
											COPY(STACK(base, 0), STACK(base, 2));
											LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 3));	/* STREAM */
											struct_typep(STACK(base, 2));
											if(CL_TRUEP(STACK(base, 2)))
											{
												LOAD_SMSTR((CL_FORM *)&Kwrite2[32], STACK(base, 2));	/* #<stream> */
												COPY(STACK(base, 1), STACK(base, 3));
												Fwrite_string(STACK(base, 2), 2);
											}
											else
											{
												COPY(STACK(base, 0), STACK(base, 2));
												LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 3));	/* PACKAGE */
												struct_typep(STACK(base, 2));
												if(CL_TRUEP(STACK(base, 2)))
												{
													LOAD_SMSTR((CL_FORM *)&Kwrite2[34], STACK(base, 2));	/* #<Package " */
													COPY(STACK(base, 1), STACK(base, 3));
													Fwrite_string(STACK(base, 2), 2);
													COPY(STACK(base, 0), STACK(base, 2));
													COPY(STACK(base, 0), STACK(base, 3));
													LOAD_FIXNUM(0, STACK(base, 4));
													LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 5));	/* PACKAGE */
													struct_ref(STACK(base, 3));
													COPY(STACK(base, 3), STACK(base, 2));
													COPY(STACK(base, 1), STACK(base, 3));
													Fwrite_string(STACK(base, 2), 2);
													LOAD_SMSTR((CL_FORM *)&Kwrite2[36], STACK(base, 2));	/* "> */
													COPY(STACK(base, 1), STACK(base, 3));
													Fwrite_string(STACK(base, 2), 2);
												}
												else
												{
													COPY(STACK(base, 0), STACK(base, 2));
													LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 3));	/* PATHNAME */
													struct_typep(STACK(base, 2));
													if(CL_TRUEP(STACK(base, 2)))
													{
														LOAD_SMSTR((CL_FORM *)&Kwrite2[38], STACK(base, 2));	/* #P" */
														COPY(STACK(base, 1), STACK(base, 3));
														Fwrite_string(STACK(base, 2), 2);
														COPY(STACK(base, 0), STACK(base, 2));
														Fnamestring(STACK(base, 2));
														mv_count = 1;
														COPY(STACK(base, 1), STACK(base, 3));
														Fwrite_string(STACK(base, 2), 2);
														LOAD_CHAR('\"', STACK(base, 2));
														COPY(STACK(base, 1), STACK(base, 3));
														Fwrite_char(STACK(base, 2), 2);
														mv_count = 1;
													}
													else
													{
														if(CL_INSTANCEP(STACK(base, 0)))
														{
															LOAD_SMSTR((CL_FORM *)&Kwrite2[40], STACK(base, 2));	/* #<INSTANCE of  */
															COPY(STACK(base, 1), STACK(base, 3));
															Fwrite_string(STACK(base, 2), 2);
															LOAD_FIXNUM(-1, STACK(base, 2));
															COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 0))), 1 + GET_FIXNUM(STACK(base, 2))), STACK(base, 2));
															LOAD_FIXNUM(0, STACK(base, 3));
															COPY(OFFSET(AR_BASE(GET_FORM(STACK(base, 2))), 1 + GET_FIXNUM(STACK(base, 3))), STACK(base, 2));
															COPY(STACK(base, 1), STACK(base, 3));
															write2(STACK(base, 2));
															LOAD_SMSTR((CL_FORM *)&Kwrite2[42], STACK(base, 2));	/* > */
															COPY(STACK(base, 1), STACK(base, 3));
															Fwrite_string(STACK(base, 2), 2);
														}
														else
														{
															COPY(STACK(base, 0), STACK(base, 2));
															structp(STACK(base, 2));
															if(CL_TRUEP(STACK(base, 2)))
															{
																if(CL_FIXNUMP(SYMVAL(Slisp, 406)) && GET_FIXNUM(SYMVAL(Slisp, 406)) == 0)	/* *PRINT-LEVEL* */
																{
																	LOAD_CHAR('#', STACK(base, 2));
																	COPY(STACK(base, 1), STACK(base, 3));
																	Fwrite_char(STACK(base, 2), 2);
																	mv_count = 1;
																}
																else
																{
																	if(CL_TRUEP(SYMVAL(Slisp, 406)))	/* *PRINT-LEVEL* */
																	{
																		COPY(SYMVAL(Slisp, 406), STACK(base, 2));	/* *PRINT-LEVEL* */
																		F1minus(STACK(base, 2));
																	}
																	else
																	{
																		COPY(SYMVAL(Slisp, 406), STACK(base, 2));	/* *PRINT-LEVEL* */
																	}
																	COPY(STACK(base, 0), STACK(base, 3));
																	struct_type(STACK(base, 3));
																	COPY(STACK(base, 2), STACK(base, 4));
																	BIND_SPECIAL(SYMBOL(Slisp, 406), STACK(base, 4));	/* *PRINT-LEVEL* */
																	LOAD_SMSTR((CL_FORM *)&Kwrite2[44], STACK(base, 5));	/* #S( */
																	COPY(STACK(base, 1), STACK(base, 6));
																	Fwrite_string(STACK(base, 5), 2);
																	COPY(STACK(base, 3), STACK(base, 5));
																	COPY(STACK(base, 1), STACK(base, 6));
																	write2(STACK(base, 5));
																	COPY(STACK(base, 0), STACK(base, 5));
																	struct_size(STACK(base, 5));
																	LOAD_FIXNUM(0, STACK(base, 6));
																	M113_1:;
																	COPY(STACK(base, 6), STACK(base, 7));
																	COPY(STACK(base, 5), STACK(base, 8));
																	Fge(STACK(base, 7), 2);
																	if(CL_TRUEP(STACK(base, 7)))
																	{
																		goto RETURN125;
																	}
																	bool_result = CL_TRUEP(SYMVAL(Slisp, 405));	/* *PRINT-LENGTH* */
																	if(bool_result)
																	{
																		COPY(STACK(base, 6), STACK(base, 7));
																		COPY(SYMVAL(Slisp, 405), STACK(base, 8));	/* *PRINT-LENGTH* */
																		Fge(STACK(base, 7), 2);
																		bool_result = CL_TRUEP(STACK(base, 7));
																	}
																	else
																	{
																		bool_result = FALSE;
																	}
																	if(bool_result)
																	{
																		LOAD_SMSTR((CL_FORM *)&Kwrite2[46], STACK(base, 7));	/*  ... */
																		COPY(STACK(base, 1), STACK(base, 8));
																		Fwrite_string(STACK(base, 7), 2);
																		goto RETURN125;
																	}
																	LOAD_SMSTR((CL_FORM *)&Kwrite2[48], STACK(base, 7));	/*  #<slot>  */
																	COPY(STACK(base, 1), STACK(base, 8));
																	Fwrite_string(STACK(base, 7), 2);
																	COPY(STACK(base, 0), STACK(base, 7));
																	COPY(STACK(base, 6), STACK(base, 8));
																	COPY(STACK(base, 3), STACK(base, 9));
																	struct_ref(STACK(base, 7));
																	COPY(STACK(base, 1), STACK(base, 8));
																	write2(STACK(base, 7));
																	COPY(STACK(base, 6), STACK(base, 7));
																	F1plus(STACK(base, 7));
																	COPY(STACK(base, 7), STACK(base, 6));
																	goto M113_1;
																	RETURN125:;
																	RESTORE_SPECIAL;
																	LOAD_CHAR(')', STACK(base, 2));
																	COPY(STACK(base, 1), STACK(base, 3));
																	Fwrite_char(STACK(base, 2), 2);
																	mv_count = 1;
																}
															}
															else
															{
																if(CL_C_CHAR_P(STACK(base, 0)))
																{
																	LOAD_T(STACK(base, 2));
																}
																else
																{
																	LOAD_NIL(STACK(base, 2));
																}
																if(CL_TRUEP(STACK(base, 2)))
																{
																	bool_result = CL_TRUEP(STACK(base, 2));
																}
																else
																{
																	bool_result = CL_C_UNSIGNED_CHAR_P(STACK(base, 0));
																}
																if(bool_result)
																{
																	LOAD_SMSTR((CL_FORM *)&Kwrite2[50], STACK(base, 2));	/* <C(char): */
																	COPY(STACK(base, 1), STACK(base, 3));
																	Fwrite_string(STACK(base, 2), 2);
																	COPY(STACK(base, 0), STACK(base, 2));
																	FFI_lisp_character(STACK(base, 2));
																	COPY(STACK(base, 1), STACK(base, 3));
																	Fwrite_char(STACK(base, 2), 2);
																	mv_count = 1;
																	LOAD_SMSTR((CL_FORM *)&Kwrite2[52], STACK(base, 2));	/* > */
																	COPY(STACK(base, 1), STACK(base, 3));
																	Fwrite_string(STACK(base, 2), 2);
																}
																else
																{
																	if(CL_C_SHORT_P(STACK(base, 0)))
																	{
																		LOAD_T(STACK(base, 2));
																	}
																	else
																	{
																		LOAD_NIL(STACK(base, 2));
																	}
																	if(CL_TRUEP(STACK(base, 2)))
																	{
																		bool_result = CL_TRUEP(STACK(base, 2));
																	}
																	else
																	{
																		if(CL_C_INT_P(STACK(base, 0)))
																		{
																			LOAD_T(STACK(base, 3));
																		}
																		else
																		{
																			LOAD_NIL(STACK(base, 3));
																		}
																		if(CL_TRUEP(STACK(base, 3)))
																		{
																			bool_result = CL_TRUEP(STACK(base, 3));
																		}
																		else
																		{
																			if(CL_C_LONG_P(STACK(base, 0)))
																			{
																				LOAD_T(STACK(base, 4));
																			}
																			else
																			{
																				LOAD_NIL(STACK(base, 4));
																			}
																			if(CL_TRUEP(STACK(base, 4)))
																			{
																				bool_result = CL_TRUEP(STACK(base, 4));
																			}
																			else
																			{
																				if(CL_C_UNSIGNED_SHORT_P(STACK(base, 0)))
																				{
																					LOAD_T(STACK(base, 5));
																				}
																				else
																				{
																					LOAD_NIL(STACK(base, 5));
																				}
																				if(CL_TRUEP(STACK(base, 5)))
																				{
																					bool_result = CL_TRUEP(STACK(base, 5));
																				}
																				else
																				{
																					if(CL_C_UNSIGNED_INT_P(STACK(base, 0)))
																					{
																						LOAD_T(STACK(base, 6));
																					}
																					else
																					{
																						LOAD_NIL(STACK(base, 6));
																					}
																					if(CL_TRUEP(STACK(base, 6)))
																					{
																						bool_result = CL_TRUEP(STACK(base, 6));
																					}
																					else
																					{
																						bool_result = CL_C_UNSIGNED_LONG_P(STACK(base, 0));
																					}
																				}
																			}
																		}
																	}
																	if(bool_result)
																	{
																		LOAD_SMSTR((CL_FORM *)&Kwrite2[54], STACK(base, 2));	/* <C(integer): */
																		COPY(STACK(base, 1), STACK(base, 3));
																		Fwrite_string(STACK(base, 2), 2);
																		COPY(STACK(base, 0), STACK(base, 2));
																		FFI_lisp_integer(STACK(base, 2));
																		COPY(STACK(base, 1), STACK(base, 3));
																		print_integer(STACK(base, 2));
																		mv_count = 1;
																		LOAD_SMSTR((CL_FORM *)&Kwrite2[56], STACK(base, 2));	/* > */
																		COPY(STACK(base, 1), STACK(base, 3));
																		Fwrite_string(STACK(base, 2), 2);
																	}
																	else
																	{
																		COPY(STACK(base, 0), STACK(base, 2));
																		c_float_p(STACK(base, 2));
																		if(CL_TRUEP(STACK(base, 2)))
																		{
																			bool_result = CL_TRUEP(STACK(base, 2));
																		}
																		else
																		{
																			COPY(STACK(base, 0), STACK(base, 3));
																			c_double_p(STACK(base, 3));
																			if(CL_TRUEP(STACK(base, 3)))
																			{
																				bool_result = CL_TRUEP(STACK(base, 3));
																			}
																			else
																			{
																				COPY(STACK(base, 0), STACK(base, 4));
																				c_long_double_p(STACK(base, 4));
																				bool_result = CL_TRUEP(STACK(base, 4));
																			}
																		}
																		if(bool_result)
																		{
																			LOAD_SMSTR((CL_FORM *)&Kwrite2[58], STACK(base, 2));	/* <C(float): */
																			COPY(STACK(base, 1), STACK(base, 3));
																			Fwrite_string(STACK(base, 2), 2);
																			COPY(STACK(base, 0), STACK(base, 2));
																			FFI_lisp_float(STACK(base, 2));
																			COPY(STACK(base, 1), STACK(base, 3));
																			print_float(STACK(base, 2));
																			LOAD_SMSTR((CL_FORM *)&Kwrite2[60], STACK(base, 2));	/* > */
																			COPY(STACK(base, 1), STACK(base, 3));
																			Fwrite_string(STACK(base, 2), 2);
																		}
																		else
																		{
																			if(CL_C_STRUCT_P(STACK(base, 0)))
																			{
																				LOAD_SMSTR((CL_FORM *)&Kwrite2[62], STACK(base, 2));	/* <C(struct)> */
																				COPY(STACK(base, 1), STACK(base, 3));
																				Fwrite_string(STACK(base, 2), 2);
																			}
																			else
																			{
																				COPY(STACK(base, 0), STACK(base, 2));
																				unbound_value_p(STACK(base, 2));
																				if(CL_TRUEP(STACK(base, 2)))
																				{
																					LOAD_SMSTR((CL_FORM *)&Kwrite2[64], STACK(base, 2));	/* #<Unbound> */
																					COPY(STACK(base, 1), STACK(base, 3));
																					Fwrite_string(STACK(base, 2), 2);
																				}
																				else
																				{
																					LOAD_SMSTR((CL_FORM *)&Kwrite2[66], STACK(base, 2));	/* #<!!! unknown type !!!!> */
																					COPY(STACK(base, 1), STACK(base, 3));
																					Fwrite_string(STACK(base, 2), 2);
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

static void Z95_write_symbol_name(CL_FORM *base, CL_FORM *display[])
{
	COPY(STACK(base, 0), STACK(base, 1));
	Flength(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	Fzerop(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&Kwrite2[2], STACK(base, 0));	/* || */
		COPY(&display[0][1], STACK(base, 1));
		Fwrite_string(STACK(base, 0), 2);
	}
	else
	{
		LOAD_NIL(STACK(base, 2));
		LOAD_FIXNUM(0, STACK(base, 3));
		M114_1:;
		COPY(STACK(base, 3), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Fge(STACK(base, 4), 2);
		if(CL_TRUEP(STACK(base, 4)))
		{
			goto RETURN126;
		}
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 5));
		Fchar(STACK(base, 4));
		Flower_case_p(STACK(base, 4));
		if(CL_TRUEP(STACK(base, 4)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(SYMVAL(Slisp, 382), STACK(base, 5));	/* *READTABLE* */
			LOAD_FIXNUM(0, STACK(base, 6));
			LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 7));	/* READTABLE */
			struct_ref(STACK(base, 5));
			COPY(STACK(base, 0), STACK(base, 6));
			COPY(STACK(base, 3), STACK(base, 7));
			Fchar(STACK(base, 6));
			Fchar_code(STACK(base, 6));
			vref(STACK(base, 5));
			bool_result = NOT(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 393));	/* CONSTITUENT */
		}
		if(bool_result)
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
			goto RETURN126;
		}
		F1plus(STACK(base, 3));
		goto M114_1;
		RETURN126:;
		if(CL_TRUEP(STACK(base, 2)))
		{
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_NIL(STACK(base, 4));
			COPY(STACK(base, 3), STACK(base, 4));
			LOAD_FIXNUM(0, STACK(base, 5));
			Fchar(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 5));
			COPY(SYMVAL(Slisp, 400), STACK(base, 6));	/* *PRINT-BASE* */
			Fdigit_char_p(STACK(base, 5), 2);
			if(CL_TRUEP(STACK(base, 5)))
			{
				bool_result = CL_TRUEP(STACK(base, 5));
			}
			else
			{
				COPY(STACK(base, 4), STACK(base, 6));
				LOAD_SMSTR((CL_FORM *)&Kwrite2[4], STACK(base, 7));	/* -.+^_ */
				Ffind(STACK(base, 6), 2);
				bool_result = CL_TRUEP(STACK(base, 6));
			}
			if(bool_result)
			{
				COPY(STACK(base, 3), STACK(base, 4));
				COPY(STACK(base, 3), STACK(base, 5));
				Flength(STACK(base, 5));
				F1minus(STACK(base, 5));
				Fchar(STACK(base, 4));
				COPY(STACK(base, 4), STACK(base, 5));
				if(CL_CHARP(STACK(base, 5)) && GET_CHAR(STACK(base, 5)) == '-')
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
					bool_result = CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '+';
				}
				if(bool_result)
				{
					LOAD_NIL(STACK(base, 2));
				}
				else
				{
					LOAD_NIL(STACK(base, 5));
					COPY(STACK(base, 3), STACK(base, 6));
					Flength(STACK(base, 6));
					LOAD_FIXNUM(0, STACK(base, 7));
					M115_1:;
					COPY(STACK(base, 7), STACK(base, 8));
					COPY(STACK(base, 6), STACK(base, 9));
					Fge(STACK(base, 8), 2);
					if(CL_TRUEP(STACK(base, 8)))
					{
						goto RETURN128;
					}
					COPY(STACK(base, 3), STACK(base, 8));
					COPY(STACK(base, 7), STACK(base, 9));
					Fchar(STACK(base, 8));
					COPY(STACK(base, 8), STACK(base, 4));
					COPY(SYMVAL(Slisp, 400), STACK(base, 9));	/* *PRINT-BASE* */
					Fdigit_char_p(STACK(base, 8), 2);
					if(CL_TRUEP(STACK(base, 8)))
					{
						LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
					}
					else
					{
						COPY(STACK(base, 4), STACK(base, 8));
						LOAD_SMSTR((CL_FORM *)&Kwrite2[6], STACK(base, 9));	/* -.eEfFdDlLsS+/^_ */
						Ffind(STACK(base, 8), 2);
						if(CL_TRUEP(STACK(base, 8)))
						{
						}
						else
						{
							LOAD_NIL(STACK(base, 2));
							goto RETURN127;
						}
					}
					F1plus(STACK(base, 7));
					goto M115_1;
					RETURN128:;
					COPY(STACK(base, 5), STACK(base, 2));
				}
			}
			else
			{
				LOAD_NIL(STACK(base, 2));
			}
			RETURN127:;
		}
		if(CL_TRUEP(STACK(base, 2)))
		{
		}
		else
		{
			LOAD_FIXNUM(0, STACK(base, 3));
			M116_1:;
			COPY(STACK(base, 3), STACK(base, 4));
			COPY(STACK(base, 1), STACK(base, 5));
			Fge(STACK(base, 4), 2);
			if(CL_TRUEP(STACK(base, 4)))
			{
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
				goto RETURN129;
			}
			COPY(STACK(base, 0), STACK(base, 4));
			COPY(STACK(base, 3), STACK(base, 5));
			Fchar(STACK(base, 4));
			if(CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '.')
			{
			}
			else
			{
				goto RETURN129;
			}
			F1plus(STACK(base, 3));
			goto M116_1;
			RETURN129:;
		}
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_CHAR('|', STACK(base, 3));
			COPY(&display[0][1], STACK(base, 4));
			Fwrite_char(STACK(base, 3), 2);
			mv_count = 1;
		}
		else
		{
			if(CL_SYMBOLP(SYMVAL(Slisp, 401)) && GET_SYMBOL(SYMVAL(Slisp, 401)) == SYMBOL(Slisp, 421))	/* *PRINT-CASE* UPCASE */
			{
			}
			else
			{
				if(CL_SYMBOLP(SYMVAL(Slisp, 401)) && GET_SYMBOL(SYMVAL(Slisp, 401)) == SYMBOL(Slisp, 429))	/* *PRINT-CASE* DOWNCASE */
				{
					COPY(STACK(base, 0), STACK(base, 3));
					Fstring_downcase(STACK(base, 3), 1);
					COPY(STACK(base, 3), STACK(base, 0));
				}
				else
				{
					if(CL_SYMBOLP(SYMVAL(Slisp, 401)) && GET_SYMBOL(SYMVAL(Slisp, 401)) == SYMBOL(Slisp, 430))	/* *PRINT-CASE* CAPITALIZE */
					{
						COPY(STACK(base, 0), STACK(base, 3));
						Fstring_capitalize(STACK(base, 3), 1);
						COPY(STACK(base, 3), STACK(base, 0));
					}
				}
			}
		}
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(&display[0][1], STACK(base, 4));
		Fwrite_string(STACK(base, 3), 2);
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_CHAR('|', STACK(base, 0));
			COPY(&display[0][1], STACK(base, 1));
			Fwrite_char(STACK(base, 0), 2);
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
}
