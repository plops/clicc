#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cformat2 = {format2, -4};

CL_INIT2(Kformat2)
{
	MAKE_STRING(31, "Not enough arguments for format"),	/* 0 */
	MAKE_STRING(41, "Syntax error at end of control-string: ~s"),	/* 2 */
	MAKE_STRING(27, "Too many at-signs supplied."),	/* 4 */
	MAKE_STRING(25, "Too many colons supplied."),	/* 6 */
	MAKE_STRING(41, "Syntax error at end of control-string: ~s"),	/* 8 */
	MAKE_STRING(13, "~~~A expected"),	/* 10 */
	MAKE_STRING(2, "()"),	/* 12 */
	MAKE_STRING(2, "()"),	/* 14 */
	MAKE_STRING(3, "ies"),	/* 16 */
	MAKE_STRING(23, "illegal directive ~~:@p"),	/* 18 */
	MAKE_STRING(23, "illegal directive ~~:@*"),	/* 20 */
	MAKE_STRING(40, "~~~a:* will skip to nonexistant argument"),	/* 22 */
	MAKE_STRING(32, "unexpected ~~; in control-string"),	/* 24 */
	MAKE_STRING(23, "Illegal directive ~~:@["),	/* 26 */
	MAKE_STRING(32, "unexpected ~~} in control-string"),	/* 28 */
	MAKE_STRING(32, "unexpected ~~} in control-string"),	/* 30 */
	MAKE_STRING(84, "illegal / unimplemented directive (~A) in ~\n                          control-string"),	/* 32 */
	MAKE_STRING(32, "unexpected end of control-string"),	/* 34 */
};

static void Z98_goto(CL_FORM *base, CL_FORM *display[]);
static void Z99_get_arg(CL_FORM *base, CL_FORM *display[]);
static void Z100_get_ctrl(CL_FORM *base, CL_FORM *display[]);
static void Z101_get_params(CL_FORM *base, CL_FORM *display[]);
static void Z102_get_at_sign_or_colon(CL_FORM *base, CL_FORM *display[]);
static void Z103_get_directive(CL_FORM *base, CL_FORM *display[]);
static void Z104_skip(CL_FORM *base, CL_FORM *display[], int nargs);
static void Z105_insert_space(CL_FORM *base, CL_FORM *display[]);
static void Z106_padding(CL_FORM *base, CL_FORM *display[], int nargs);

void format2(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	switch(nargs)
	{
		case 3:
		LOAD_FIXNUM(0, STACK(base, 3));
		case 4:
		LOAD_NIL(STACK(base, 4));
		case 5:
		LOAD_NIL(STACK(base, 5));
		case 6:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 2), STACK(base, 6));
	LOAD_FIXNUM(0, STACK(base, 7));
	LOAD_NIL(STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	Flength(STACK(base, 9));
	LOAD_NIL(STACK(base, 10));
	LOAD_NIL(STACK(base, 11));
	LOAD_NIL(STACK(base, 12));
	LOAD_NIL(STACK(base, 13));
	LOAD_NIL(STACK(base, 14));
	M118_1:;
	display[0] = STACK(base, 0);
	Z100_get_ctrl(STACK(base, 15), display);
	COPY(STACK(base, 15), STACK(base, 10));
	if(CL_TRUEP(STACK(base, 10)))
	{
	}
	else
	{
		if(CL_TRUEP(STACK(base, 4)))
		{
			LOAD_SMSTR((CL_FORM *)&Kformat2[10], STACK(base, 15));	/* ~~~A expected */
			COPY(STACK(base, 4), STACK(base, 16));
			Ferror(STACK(base, 15), 2);
		}
		goto RETURN132;
	}
	if(CL_CHARP(STACK(base, 10)) && GET_CHAR(STACK(base, 10)) == '~')
	{
		display[0] = STACK(base, 0);
		Z101_get_params(STACK(base, 15), display);
		COPY(STACK(base, 11), STACK(base, 15));
		COPY(STACK(base, 15), STACK(base, 16));
		Fcar(STACK(base, 16));
		COPY(STACK(base, 16), STACK(base, 12));
		display[0] = STACK(base, 0);
		Z102_get_at_sign_or_colon(STACK(base, 15), display);
		display[0] = STACK(base, 0);
		Z103_get_directive(STACK(base, 15), display);
		COPY(STACK(base, 15), STACK(base, 10));
		Fchar_upcase(STACK(base, 15));
		if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == 'A')
		{
			display[0] = STACK(base, 0);
			Z99_get_arg(STACK(base, 16), display);
			if(CL_TRUEP(STACK(base, 14)))
			{
				bool_result = NOT(CL_TRUEP(STACK(base, 16)));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				LOAD_SMSTR((CL_FORM *)&Kformat2[12], STACK(base, 17));	/* () */
			}
			else
			{
				Fmake_string_output_stream(STACK(base, 17), 0);
				COPY(STACK(base, 16), STACK(base, 18));
				COPY(STACK(base, 17), STACK(base, 19));
				Fprinc(STACK(base, 18), 2);
				COPY(STACK(base, 17), STACK(base, 18));
				Fget_output_stream_string(STACK(base, 18));
				COPY(STACK(base, 18), STACK(base, 17));
			}
			COPY(STACK(base, 17), STACK(base, 18));
			Flength(STACK(base, 18));
			if(CL_TRUEP(STACK(base, 13)))
			{
				DOWN_FUNARG downfun1;
				downfun1.fun = Z106_padding;
				downfun1.par_spec = -2;
				downfun1.display = display;
				LOAD_DOWNFUN(&downfun1, STACK(base, 19));
				COPY(STACK(base, 18), STACK(base, 20));
				COPY(STACK(base, 11), STACK(base, 21));
				display[0] = STACK(base, 0);
				Fapply(STACK(base, 19), 3);
				mv_count = 1;
				COPY(STACK(base, 17), STACK(base, 19));
				COPY(STACK(base, 0), STACK(base, 20));
				Fprinc(STACK(base, 19), 2);
			}
			else
			{
				COPY(STACK(base, 17), STACK(base, 19));
				COPY(STACK(base, 0), STACK(base, 20));
				Fprinc(STACK(base, 19), 2);
				{
					DOWN_FUNARG downfun1;
					downfun1.fun = Z106_padding;
					downfun1.par_spec = -2;
					downfun1.display = display;
					LOAD_DOWNFUN(&downfun1, STACK(base, 19));
					COPY(STACK(base, 18), STACK(base, 20));
					COPY(STACK(base, 11), STACK(base, 21));
					display[0] = STACK(base, 0);
					Fapply(STACK(base, 19), 3);
					mv_count = 1;
				}
			}
		}
		else
		{
			if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == 'S')
			{
				display[0] = STACK(base, 0);
				Z99_get_arg(STACK(base, 16), display);
				if(CL_TRUEP(STACK(base, 14)))
				{
					bool_result = NOT(CL_TRUEP(STACK(base, 16)));
				}
				else
				{
					bool_result = FALSE;
				}
				if(bool_result)
				{
					LOAD_SMSTR((CL_FORM *)&Kformat2[14], STACK(base, 17));	/* () */
				}
				else
				{
					Fmake_string_output_stream(STACK(base, 17), 0);
					COPY(STACK(base, 16), STACK(base, 18));
					COPY(STACK(base, 17), STACK(base, 19));
					Fprin1(STACK(base, 18), 2);
					COPY(STACK(base, 17), STACK(base, 18));
					Fget_output_stream_string(STACK(base, 18));
					COPY(STACK(base, 18), STACK(base, 17));
				}
				COPY(STACK(base, 17), STACK(base, 18));
				Flength(STACK(base, 18));
				if(CL_TRUEP(STACK(base, 13)))
				{
					DOWN_FUNARG downfun1;
					downfun1.fun = Z106_padding;
					downfun1.par_spec = -2;
					downfun1.display = display;
					LOAD_DOWNFUN(&downfun1, STACK(base, 19));
					COPY(STACK(base, 18), STACK(base, 20));
					COPY(STACK(base, 11), STACK(base, 21));
					display[0] = STACK(base, 0);
					Fapply(STACK(base, 19), 3);
					mv_count = 1;
					COPY(STACK(base, 17), STACK(base, 19));
					COPY(STACK(base, 0), STACK(base, 20));
					Fprinc(STACK(base, 19), 2);
				}
				else
				{
					COPY(STACK(base, 17), STACK(base, 19));
					COPY(STACK(base, 0), STACK(base, 20));
					Fprinc(STACK(base, 19), 2);
					{
						DOWN_FUNARG downfun1;
						downfun1.fun = Z106_padding;
						downfun1.par_spec = -2;
						downfun1.display = display;
						LOAD_DOWNFUN(&downfun1, STACK(base, 19));
						COPY(STACK(base, 18), STACK(base, 20));
						COPY(STACK(base, 11), STACK(base, 21));
						display[0] = STACK(base, 0);
						Fapply(STACK(base, 19), 3);
						mv_count = 1;
					}
				}
			}
			else
			{
				if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == 'D')
				{
					COPY(STACK(base, 0), STACK(base, 16));
					stream_column(STACK(base, 16));
					Ffuncall(STACK(base, 16), 1);
					mv_count = 1;
					display[0] = STACK(base, 0);
					Z99_get_arg(STACK(base, 17), display);
					LOAD_NIL(STACK(base, 18));
					BIND_SPECIAL(SYMBOL(Slisp, 408), STACK(base, 18));	/* *PRINT-RADIX* */
					LOAD_FIXNUM(10, STACK(base, 19));
					BIND_SPECIAL(SYMBOL(Slisp, 400), STACK(base, 19));	/* *PRINT-BASE* */
					if(CL_FIXNUMP(STACK(base, 17)))
					{
						if(CL_TRUEP(STACK(base, 13)))
						{
							COPY(STACK(base, 17), STACK(base, 20));
							Fplusp(STACK(base, 20));
							bool_result = CL_TRUEP(STACK(base, 20));
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
						LOAD_CHAR('+', STACK(base, 20));
						COPY(STACK(base, 0), STACK(base, 21));
						Fwrite_char(STACK(base, 20), 2);
						mv_count = 1;
					}
					COPY(STACK(base, 17), STACK(base, 20));
					COPY(STACK(base, 0), STACK(base, 21));
					Fprinc(STACK(base, 20), 2);
					COPY(STACK(base, 12), STACK(base, 20));
					COPY(STACK(base, 16), STACK(base, 21));
					display[0] = STACK(base, 0);
					Z105_insert_space(STACK(base, 20), display);
					RESTORE_SPECIAL;
					RESTORE_SPECIAL;
				}
				else
				{
					if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == 'B')
					{
						COPY(STACK(base, 0), STACK(base, 16));
						stream_column(STACK(base, 16));
						Ffuncall(STACK(base, 16), 1);
						mv_count = 1;
						LOAD_NIL(STACK(base, 17));
						BIND_SPECIAL(SYMBOL(Slisp, 408), STACK(base, 17));	/* *PRINT-RADIX* */
						LOAD_FIXNUM(2, STACK(base, 18));
						BIND_SPECIAL(SYMBOL(Slisp, 400), STACK(base, 18));	/* *PRINT-BASE* */
						display[0] = STACK(base, 0);
						Z99_get_arg(STACK(base, 19), display);
						COPY(STACK(base, 0), STACK(base, 20));
						Fprinc(STACK(base, 19), 2);
						COPY(STACK(base, 12), STACK(base, 19));
						COPY(STACK(base, 16), STACK(base, 20));
						display[0] = STACK(base, 0);
						Z105_insert_space(STACK(base, 19), display);
						RESTORE_SPECIAL;
						RESTORE_SPECIAL;
					}
					else
					{
						if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == 'O')
						{
							COPY(STACK(base, 0), STACK(base, 16));
							stream_column(STACK(base, 16));
							Ffuncall(STACK(base, 16), 1);
							mv_count = 1;
							LOAD_NIL(STACK(base, 17));
							BIND_SPECIAL(SYMBOL(Slisp, 408), STACK(base, 17));	/* *PRINT-RADIX* */
							LOAD_FIXNUM(8, STACK(base, 18));
							BIND_SPECIAL(SYMBOL(Slisp, 400), STACK(base, 18));	/* *PRINT-BASE* */
							display[0] = STACK(base, 0);
							Z99_get_arg(STACK(base, 19), display);
							COPY(STACK(base, 0), STACK(base, 20));
							Fprinc(STACK(base, 19), 2);
							COPY(STACK(base, 12), STACK(base, 19));
							COPY(STACK(base, 16), STACK(base, 20));
							display[0] = STACK(base, 0);
							Z105_insert_space(STACK(base, 19), display);
							RESTORE_SPECIAL;
							RESTORE_SPECIAL;
						}
						else
						{
							if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == 'P')
							{
								if(CL_TRUEP(STACK(base, 14)))
								{
									COPY(STACK(base, 8), STACK(base, 16));
								}
								else
								{
									display[0] = STACK(base, 0);
									Z99_get_arg(STACK(base, 16), display);
								}
								if(CL_FIXNUMP(STACK(base, 16)) && GET_FIXNUM(STACK(base, 16)) == 1)
								{
									if(CL_TRUEP(STACK(base, 13)))
									{
										LOAD_CHAR('y', STACK(base, 17));
										COPY(STACK(base, 0), STACK(base, 18));
										Fwrite_char(STACK(base, 17), 2);
										mv_count = 1;
									}
								}
								else
								{
									if(CL_TRUEP(STACK(base, 13)))
									{
										LOAD_SMSTR((CL_FORM *)&Kformat2[16], STACK(base, 17));	/* ies */
										COPY(STACK(base, 0), STACK(base, 18));
										Fprinc(STACK(base, 17), 2);
									}
									else
									{
										LOAD_CHAR('s', STACK(base, 17));
										COPY(STACK(base, 0), STACK(base, 18));
										Fwrite_char(STACK(base, 17), 2);
										mv_count = 1;
									}
								}
							}
							else
							{
								if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == 'F')
								{
									COPY(STACK(base, 0), STACK(base, 16));
									stream_column(STACK(base, 16));
									Ffuncall(STACK(base, 16), 1);
									mv_count = 1;
									display[0] = STACK(base, 0);
									Z99_get_arg(STACK(base, 17), display);
									COPY(STACK(base, 0), STACK(base, 18));
									Fprinc(STACK(base, 17), 2);
									COPY(STACK(base, 12), STACK(base, 17));
									COPY(STACK(base, 16), STACK(base, 18));
									display[0] = STACK(base, 0);
									Z105_insert_space(STACK(base, 17), display);
								}
								else
								{
									if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '%')
									{
										if(CL_NUMBERP(STACK(base, 12)))
										{
											LOAD_FIXNUM(0, STACK(base, 16));
											M119_1:;
											COPY(STACK(base, 16), STACK(base, 17));
											COPY(STACK(base, 12), STACK(base, 18));
											Fge(STACK(base, 17), 2);
											if(CL_TRUEP(STACK(base, 17)))
											{
												goto RETURN133;
											}
											COPY(STACK(base, 0), STACK(base, 17));
											Fterpri(STACK(base, 17), 1);
											COPY(STACK(base, 16), STACK(base, 17));
											F1plus(STACK(base, 17));
											COPY(STACK(base, 17), STACK(base, 16));
											goto M119_1;
											RETURN133:;
										}
										else
										{
											COPY(STACK(base, 0), STACK(base, 16));
											Fterpri(STACK(base, 16), 1);
										}
									}
									else
									{
										if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '&')
										{
											if(CL_NUMBERP(STACK(base, 12)))
											{
												COPY(STACK(base, 12), STACK(base, 16));
												Fzerop(STACK(base, 16));
												if(CL_TRUEP(STACK(base, 16)))
												{
												}
												else
												{
													COPY(STACK(base, 0), STACK(base, 16));
													Ffresh_line(STACK(base, 16), 1);
													mv_count = 1;
													COPY(STACK(base, 12), STACK(base, 16));
													F1minus(STACK(base, 16));
													LOAD_FIXNUM(0, STACK(base, 17));
													M120_1:;
													COPY(STACK(base, 17), STACK(base, 18));
													COPY(STACK(base, 16), STACK(base, 19));
													Fge(STACK(base, 18), 2);
													if(CL_TRUEP(STACK(base, 18)))
													{
														goto RETURN134;
													}
													COPY(STACK(base, 0), STACK(base, 18));
													Fterpri(STACK(base, 18), 1);
													COPY(STACK(base, 17), STACK(base, 18));
													F1plus(STACK(base, 18));
													COPY(STACK(base, 18), STACK(base, 17));
													goto M120_1;
													RETURN134:;
												}
											}
											else
											{
												COPY(STACK(base, 0), STACK(base, 16));
												Ffresh_line(STACK(base, 16), 1);
												mv_count = 1;
											}
										}
										else
										{
											if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '|')
											{
												if(CL_NUMBERP(STACK(base, 12)))
												{
													LOAD_FIXNUM(0, STACK(base, 16));
													M121_1:;
													COPY(STACK(base, 16), STACK(base, 17));
													COPY(STACK(base, 12), STACK(base, 18));
													Fge(STACK(base, 17), 2);
													if(CL_TRUEP(STACK(base, 17)))
													{
														goto RETURN135;
													}
													LOAD_CHAR('\f', STACK(base, 17));
													COPY(STACK(base, 0), STACK(base, 18));
													Fwrite_char(STACK(base, 17), 2);
													mv_count = 1;
													COPY(STACK(base, 16), STACK(base, 17));
													F1plus(STACK(base, 17));
													COPY(STACK(base, 17), STACK(base, 16));
													goto M121_1;
													RETURN135:;
												}
												else
												{
													LOAD_CHAR('\f', STACK(base, 16));
													COPY(STACK(base, 0), STACK(base, 17));
													Fwrite_char(STACK(base, 16), 2);
													mv_count = 1;
												}
											}
											else
											{
												if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '~')
												{
													if(CL_NUMBERP(STACK(base, 12)))
													{
														LOAD_FIXNUM(0, STACK(base, 16));
														M122_1:;
														COPY(STACK(base, 16), STACK(base, 17));
														COPY(STACK(base, 12), STACK(base, 18));
														Fge(STACK(base, 17), 2);
														if(CL_TRUEP(STACK(base, 17)))
														{
															goto RETURN136;
														}
														LOAD_CHAR('~', STACK(base, 17));
														COPY(STACK(base, 0), STACK(base, 18));
														Fwrite_char(STACK(base, 17), 2);
														mv_count = 1;
														COPY(STACK(base, 16), STACK(base, 17));
														F1plus(STACK(base, 17));
														COPY(STACK(base, 17), STACK(base, 16));
														goto M122_1;
														RETURN136:;
													}
													else
													{
														LOAD_CHAR('~', STACK(base, 16));
														COPY(STACK(base, 0), STACK(base, 17));
														Fwrite_char(STACK(base, 16), 2);
														mv_count = 1;
													}
												}
												else
												{
													if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '\n')
													{
														if(CL_TRUEP(STACK(base, 13)))
														{
															if(CL_TRUEP(STACK(base, 14)))
															{
																LOAD_SMSTR((CL_FORM *)&Kformat2[18], STACK(base, 16));	/* illegal directive ~~:@p */
																Ferror(STACK(base, 16), 1);
															}
															COPY(STACK(base, 0), STACK(base, 16));
															Fterpri(STACK(base, 16), 1);
														}
														if(CL_TRUEP(STACK(base, 14)))
														{
														}
														else
														{
															M123_1:;
															display[0] = STACK(base, 0);
															Z100_get_ctrl(STACK(base, 16), display);
															COPY(STACK(base, 16), STACK(base, 10));
															if(CL_TRUEP(STACK(base, 10)))
															{
															}
															else
															{
																goto RETURN137;
															}
															if(CL_CHARP(STACK(base, 10)) && GET_CHAR(STACK(base, 10)) == ' ')
															{
															}
															else
															{
																COPY(STACK(base, 3), STACK(base, 16));
																F1minus(STACK(base, 16));
																COPY(STACK(base, 16), STACK(base, 3));
																goto RETURN137;
															}
															goto M123_1;
															RETURN137:;
														}
													}
													else
													{
														if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '*')
														{
															if(CL_TRUEP(STACK(base, 13)))
															{
																if(CL_TRUEP(STACK(base, 14)))
																{
																	LOAD_SMSTR((CL_FORM *)&Kformat2[20], STACK(base, 16));	/* illegal directive ~~:@* */
																	Ferror(STACK(base, 16), 1);
																}
																if(CL_NUMBERP(STACK(base, 12)))
																{
																}
																else
																{
																	LOAD_FIXNUM(0, STACK(base, 12));
																}
																COPY(STACK(base, 12), STACK(base, 16));
																display[0] = STACK(base, 0);
																Z98_goto(STACK(base, 16), display);
															}
															else
															{
																if(CL_TRUEP(STACK(base, 14)))
																{
																	if(CL_NUMBERP(STACK(base, 12)))
																	{
																	}
																	else
																	{
																		LOAD_FIXNUM(1, STACK(base, 12));
																	}
																	COPY(STACK(base, 12), STACK(base, 16));
																	COPY(STACK(base, 7), STACK(base, 17));
																	Fgt(STACK(base, 16), 2);
																	if(CL_TRUEP(STACK(base, 16)))
																	{
																		LOAD_SMSTR((CL_FORM *)&Kformat2[22], STACK(base, 16));	/* ~~~a:* will skip to nonexistant argument */
																		COPY(STACK(base, 12), STACK(base, 17));
																		Ferror(STACK(base, 16), 2);
																	}
																	COPY(STACK(base, 7), STACK(base, 16));
																	COPY(STACK(base, 12), STACK(base, 17));
																	Fminus(STACK(base, 16), 2);
																	display[0] = STACK(base, 0);
																	Z98_goto(STACK(base, 16), display);
																}
																else
																{
																	if(CL_NUMBERP(STACK(base, 12)))
																	{
																	}
																	else
																	{
																		LOAD_FIXNUM(1, STACK(base, 12));
																	}
																	LOAD_FIXNUM(0, STACK(base, 16));
																	M124_1:;
																	COPY(STACK(base, 16), STACK(base, 17));
																	COPY(STACK(base, 12), STACK(base, 18));
																	Fge(STACK(base, 17), 2);
																	if(CL_TRUEP(STACK(base, 17)))
																	{
																		goto RETURN138;
																	}
																	display[0] = STACK(base, 0);
																	Z99_get_arg(STACK(base, 17), display);
																	COPY(STACK(base, 16), STACK(base, 17));
																	F1plus(STACK(base, 17));
																	COPY(STACK(base, 17), STACK(base, 16));
																	goto M124_1;
																	RETURN138:;
																}
															}
														}
														else
														{
															if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '?')
															{
																COPY(STACK(base, 0), STACK(base, 16));
																display[0] = STACK(base, 0);
																Z99_get_arg(STACK(base, 17), display);
																display[0] = STACK(base, 0);
																Z99_get_arg(STACK(base, 18), display);
																format2(STACK(base, 16), 3);
																mv_count = 1;
															}
															else
															{
																if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == ';')
																{
																	if(CL_CHARP(STACK(base, 5)) && GET_CHAR(STACK(base, 5)) == ';')
																	{
																		LOAD_T(STACK(base, 16));
																	}
																	else
																	{
																		LOAD_NIL(STACK(base, 16));
																	}
																	if(CL_TRUEP(STACK(base, 16)))
																	{
																		bool_result = TRUE;
																	}
																	else
																	{
																		bool_result = CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == ';';
																	}
																	if(bool_result)
																	{
																		COPY(STACK(base, 6), STACK(base, 16));
																		COPY(STACK(base, 3), STACK(base, 17));
																		LOAD_CHAR(';', STACK(base, 18));
																		COPY(STACK(base, 16), STACK(base, 0));
																		COPY(STACK(base, 17), &mv_buf[0]);
																		COPY(STACK(base, 18), &mv_buf[1]);
																		mv_count = 3;
																		goto RETURN131;
																	}
																	else
																	{
																		LOAD_SMSTR((CL_FORM *)&Kformat2[24], STACK(base, 16));	/* unexpected ~~; in control-string */
																		Ferror(STACK(base, 16), 1);
																	}
																}
																else
																{
																	if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '[')
																	{
																		if(CL_TRUEP(STACK(base, 14)))
																		{
																			bool_result = CL_TRUEP(STACK(base, 13));
																		}
																		else
																		{
																			bool_result = FALSE;
																		}
																		if(bool_result)
																		{
																			LOAD_SMSTR((CL_FORM *)&Kformat2[26], STACK(base, 16));	/* Illegal directive ~~:@[ */
																			Ferror(STACK(base, 16), 1);
																		}
																		else
																		{
																			if(CL_TRUEP(STACK(base, 14)))
																			{
																				display[0] = STACK(base, 0);
																				Z99_get_arg(STACK(base, 16), display);
																				if(CL_TRUEP(STACK(base, 16)))
																				{
																					LOAD_CHAR(';', STACK(base, 16));
																					display[0] = STACK(base, 0);
																					Z104_skip(STACK(base, 16), display, 1);
																					COPY(STACK(base, 0), STACK(base, 16));
																					COPY(STACK(base, 1), STACK(base, 17));
																					COPY(STACK(base, 6), STACK(base, 18));
																					COPY(STACK(base, 3), STACK(base, 19));
																					LOAD_CHAR(']', STACK(base, 20));
																					format2(STACK(base, 16), 5);
																					MV_TO_STACK(mv_count, 17);
																					{
																						int nargs;
																						nargs = mv_count;
																						mv_count = 1;
																						switch(nargs)
																						{
																							case 0:
																							LOAD_NIL(STACK(base, 16));
																							case 1:
																							LOAD_NIL(STACK(base, 17));
																							nargs = 2;
																						}
																						Flist(STACK(base, 18), nargs - 2);
																						COPY(STACK(base, 16), STACK(base, 6));
																						COPY(STACK(base, 17), STACK(base, 3));
																					}
																				}
																				else
																				{
																					COPY(STACK(base, 0), STACK(base, 16));
																					COPY(STACK(base, 1), STACK(base, 17));
																					COPY(STACK(base, 6), STACK(base, 18));
																					COPY(STACK(base, 3), STACK(base, 19));
																					LOAD_CHAR(';', STACK(base, 20));
																					format2(STACK(base, 16), 5);
																					MV_TO_STACK(mv_count, 17);
																					{
																						int nargs;
																						nargs = mv_count;
																						mv_count = 1;
																						switch(nargs)
																						{
																							case 0:
																							LOAD_NIL(STACK(base, 16));
																							case 1:
																							LOAD_NIL(STACK(base, 17));
																							nargs = 2;
																						}
																						Flist(STACK(base, 18), nargs - 2);
																						COPY(STACK(base, 16), STACK(base, 6));
																						COPY(STACK(base, 17), STACK(base, 3));
																					}
																					LOAD_CHAR(']', STACK(base, 16));
																					display[0] = STACK(base, 0);
																					Z104_skip(STACK(base, 16), display, 1);
																				}
																			}
																			else
																			{
																				if(CL_TRUEP(STACK(base, 13)))
																				{
																					COPY(STACK(base, 6), STACK(base, 16));
																					display[0] = STACK(base, 0);
																					Z99_get_arg(STACK(base, 17), display);
																					if(CL_TRUEP(STACK(base, 17)))
																					{
																						COPY(STACK(base, 0), STACK(base, 17));
																						COPY(STACK(base, 1), STACK(base, 18));
																						COPY(STACK(base, 16), STACK(base, 19));
																						COPY(STACK(base, 3), STACK(base, 20));
																						LOAD_CHAR(']', STACK(base, 21));
																						format2(STACK(base, 17), 5);
																						MV_TO_STACK(mv_count, 18);
																						{
																							int nargs;
																							nargs = mv_count;
																							mv_count = 1;
																							switch(nargs)
																							{
																								case 0:
																								LOAD_NIL(STACK(base, 17));
																								case 1:
																								LOAD_NIL(STACK(base, 18));
																								nargs = 2;
																							}
																							Flist(STACK(base, 19), nargs - 2);
																							COPY(STACK(base, 17), STACK(base, 6));
																							COPY(STACK(base, 18), STACK(base, 3));
																						}
																					}
																				}
																				else
																				{
																					if(CL_TRUEP(STACK(base, 12)))
																					{
																						COPY(STACK(base, 12), STACK(base, 16));
																					}
																					else
																					{
																						display[0] = STACK(base, 0);
																						Z99_get_arg(STACK(base, 16), display);
																					}
																					M125_1:;
																					if(CL_FIXNUMP(STACK(base, 16)) && GET_FIXNUM(STACK(base, 16)) == 0)
																					{
																						LOAD_NIL(STACK(base, 17));
																						COPY(STACK(base, 0), STACK(base, 18));
																						COPY(STACK(base, 1), STACK(base, 19));
																						COPY(STACK(base, 6), STACK(base, 20));
																						COPY(STACK(base, 3), STACK(base, 21));
																						LOAD_CHAR(']', STACK(base, 22));
																						LOAD_CHAR(';', STACK(base, 23));
																						format2(STACK(base, 18), 6);
																						MV_TO_STACK(mv_count, 19);
																						{
																							int nargs;
																							nargs = mv_count;
																							mv_count = 1;
																							switch(nargs)
																							{
																								case 0:
																								LOAD_NIL(STACK(base, 18));
																								case 1:
																								LOAD_NIL(STACK(base, 19));
																								case 2:
																								LOAD_NIL(STACK(base, 20));
																								nargs = 3;
																							}
																							Flist(STACK(base, 21), nargs - 3);
																							COPY(STACK(base, 18), STACK(base, 6));
																							COPY(STACK(base, 19), STACK(base, 3));
																							COPY(STACK(base, 20), STACK(base, 17));
																						}
																						if(CL_CHARP(STACK(base, 17)) && GET_CHAR(STACK(base, 17)) == ';')
																						{
																							LOAD_CHAR(']', STACK(base, 18));
																							display[0] = STACK(base, 0);
																							Z104_skip(STACK(base, 18), display, 1);
																						}
																						goto RETURN139;
																					}
																					LOAD_CHAR(']', STACK(base, 17));
																					LOAD_CHAR(';', STACK(base, 18));
																					display[0] = STACK(base, 0);
																					Z104_skip(STACK(base, 17), display, 2);
																					if(CL_CHARP(STACK(base, 17)) && GET_CHAR(STACK(base, 17)) == ';')
																					{
																						if(CL_TRUEP(STACK(base, 14)))
																						{
																							COPY(STACK(base, 0), STACK(base, 17));
																							COPY(STACK(base, 1), STACK(base, 18));
																							COPY(STACK(base, 6), STACK(base, 19));
																							COPY(STACK(base, 3), STACK(base, 20));
																							LOAD_CHAR(']', STACK(base, 21));
																							format2(STACK(base, 17), 5);
																							MV_TO_STACK(mv_count, 18);
																							{
																								int nargs;
																								nargs = mv_count;
																								mv_count = 1;
																								switch(nargs)
																								{
																									case 0:
																									LOAD_NIL(STACK(base, 17));
																									case 1:
																									LOAD_NIL(STACK(base, 18));
																									nargs = 2;
																								}
																								Flist(STACK(base, 19), nargs - 2);
																								COPY(STACK(base, 17), STACK(base, 6));
																								COPY(STACK(base, 18), STACK(base, 3));
																							}
																							goto RETURN139;
																						}
																						else
																						{
																							COPY(STACK(base, 16), STACK(base, 17));
																							F1minus(STACK(base, 17));
																							COPY(STACK(base, 17), STACK(base, 16));
																						}
																					}
																					else
																					{
																						goto RETURN139;
																					}
																					goto M125_1;
																					RETURN139:;
																				}
																			}
																		}
																	}
																	else
																	{
																		if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == ']')
																		{
																			if(CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == ']')
																			{
																				COPY(STACK(base, 6), STACK(base, 16));
																				COPY(STACK(base, 3), STACK(base, 17));
																				LOAD_CHAR(']', STACK(base, 18));
																				COPY(STACK(base, 16), STACK(base, 0));
																				COPY(STACK(base, 17), &mv_buf[0]);
																				COPY(STACK(base, 18), &mv_buf[1]);
																				mv_count = 3;
																				goto RETURN131;
																			}
																			else
																			{
																				LOAD_SMSTR((CL_FORM *)&Kformat2[28], STACK(base, 16));	/* unexpected ~~} in control-string */
																				Ferror(STACK(base, 16), 1);
																			}
																		}
																		else
																		{
																			if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '{')
																			{
																				display[0] = STACK(base, 0);
																				Z99_get_arg(STACK(base, 16), display);
																				LOAD_NIL(STACK(base, 17));
																				if(CL_TRUEP(STACK(base, 12)))
																				{
																				}
																				else
																				{
																					LOAD_FIXNUM(-1, STACK(base, 12));
																				}
																				M126_1:;
																				if(CL_TRUEP(STACK(base, 16)))
																				{
																				}
																				else
																				{
																					goto RETURN140;
																				}
																				if(CL_FIXNUMP(STACK(base, 12)) && GET_FIXNUM(STACK(base, 12)) == 0)
																				{
																					goto RETURN140;
																				}
																				COPY(STACK(base, 0), STACK(base, 18));
																				COPY(STACK(base, 1), STACK(base, 19));
																				COPY(STACK(base, 16), STACK(base, 20));
																				COPY(STACK(base, 3), STACK(base, 21));
																				LOAD_CHAR('}', STACK(base, 22));
																				format2(STACK(base, 18), 5);
																				MV_TO_STACK(mv_count, 19);
																				{
																					int nargs;
																					nargs = mv_count;
																					mv_count = 1;
																					switch(nargs)
																					{
																						case 0:
																						LOAD_NIL(STACK(base, 18));
																						case 1:
																						LOAD_NIL(STACK(base, 19));
																						nargs = 2;
																					}
																					Flist(STACK(base, 20), nargs - 2);
																					COPY(STACK(base, 18), STACK(base, 16));
																					COPY(STACK(base, 19), STACK(base, 17));
																				}
																				COPY(STACK(base, 12), STACK(base, 18));
																				F1minus(STACK(base, 18));
																				COPY(STACK(base, 18), STACK(base, 12));
																				goto M126_1;
																				RETURN140:;
																				if(CL_TRUEP(STACK(base, 17)))
																				{
																					COPY(STACK(base, 17), STACK(base, 3));
																				}
																				else
																				{
																					COPY(STACK(base, 3), STACK(base, 18));
																					LOAD_CHAR('}', STACK(base, 19));
																					display[0] = STACK(base, 0);
																					Z104_skip(STACK(base, 19), display, 1);
																					if(CL_TRUEP(STACK(base, 14)))
																					{
																						bool_result = NOT(CL_FIXNUMP(STACK(base, 12)) && GET_FIXNUM(STACK(base, 12)) == 0);
																					}
																					else
																					{
																						bool_result = FALSE;
																					}
																					if(bool_result)
																					{
																						COPY(STACK(base, 0), STACK(base, 19));
																						COPY(STACK(base, 1), STACK(base, 20));
																						COPY(STACK(base, 16), STACK(base, 21));
																						COPY(STACK(base, 18), STACK(base, 22));
																						LOAD_CHAR('}', STACK(base, 23));
																						format2(STACK(base, 19), 5);
																						MV_TO_STACK(mv_count, 20);
																						{
																							int nargs;
																							nargs = mv_count;
																							mv_count = 1;
																							switch(nargs)
																							{
																								case 0:
																								LOAD_NIL(STACK(base, 19));
																								case 1:
																								LOAD_NIL(STACK(base, 20));
																								nargs = 2;
																							}
																							Flist(STACK(base, 21), nargs - 2);
																							COPY(STACK(base, 19), STACK(base, 16));
																							COPY(STACK(base, 20), STACK(base, 17));
																						}
																					}
																				}
																			}
																			else
																			{
																				if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '}')
																				{
																					if(CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '}')
																					{
																						COPY(STACK(base, 6), STACK(base, 16));
																						COPY(STACK(base, 3), STACK(base, 17));
																						COPY(STACK(base, 16), STACK(base, 0));
																						COPY(STACK(base, 17), &mv_buf[0]);
																						mv_count = 2;
																						goto RETURN131;
																					}
																					else
																					{
																						LOAD_SMSTR((CL_FORM *)&Kformat2[30], STACK(base, 16));	/* unexpected ~~} in control-string */
																						Ferror(STACK(base, 16), 1);
																					}
																				}
																				else
																				{
																					LOAD_SMSTR((CL_FORM *)&Kformat2[32], STACK(base, 16));	/* illegal / unimplemented directive (~A) in ~
                          control-string */
																					COPY(STACK(base, 10), STACK(base, 17));
																					Ferror(STACK(base, 16), 2);
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
	else
	{
		COPY(STACK(base, 10), STACK(base, 15));
		COPY(STACK(base, 0), STACK(base, 16));
		Fwrite_char(STACK(base, 15), 2);
		mv_count = 1;
	}
	goto M118_1;
	RETURN132:;
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_SMSTR((CL_FORM *)&Kformat2[34], STACK(base, 6));	/* unexpected end of control-string */
		Ferror(STACK(base, 6), 1);
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
	RETURN131:;
}

static void Z98_goto(CL_FORM *base, CL_FORM *display[])
{
	if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) == 0)
	{
		COPY(&display[0][2], &display[0][6]);
		LOAD_FIXNUM(0, &display[0][7]);
		LOAD_NIL(&display[0][8]);
		COPY(&display[0][8], STACK(base, 0));
	}
	else
	{
		COPY(&display[0][2], STACK(base, 1));
		COPY(STACK(base, 0), STACK(base, 2));
		F1minus(STACK(base, 2));
		Fnthcdr(STACK(base, 1));
		COPY(STACK(base, 1), &display[0][6]);
		COPY(STACK(base, 0), &display[0][7]);
		Z99_get_arg(STACK(base, 0), display);
	}
}

static void Z99_get_arg(CL_FORM *base, CL_FORM *display[])
{
	if(CL_TRUEP(&display[0][6]))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&Kformat2[0], STACK(base, 0));	/* Not enough arguments for format */
		Ferror(STACK(base, 0), 1);
	}
	COPY(&display[0][7], STACK(base, 0));
	F1plus(STACK(base, 0));
	COPY(STACK(base, 0), &display[0][7]);
	COPY(&display[0][6], STACK(base, 0));
	Fcar(STACK(base, 0));
	COPY(&display[0][6], STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 2));
	Fcdr(STACK(base, 2));
	COPY(STACK(base, 2), &display[0][6]);
	COPY(STACK(base, 0), &display[0][8]);
}

static void Z100_get_ctrl(CL_FORM *base, CL_FORM *display[])
{
	COPY(&display[0][3], STACK(base, 0));
	COPY(&display[0][9], STACK(base, 1));
	Fge(STACK(base, 0), 2);
	if(CL_TRUEP(STACK(base, 0)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(&display[0][1], STACK(base, 0));
		COPY(&display[0][3], STACK(base, 1));
		Fchar(STACK(base, 0));
		COPY(&display[0][3], STACK(base, 1));
		F1plus(STACK(base, 1));
		COPY(STACK(base, 1), &display[0][3]);
	}
}

static void Z101_get_params(CL_FORM *base, CL_FORM *display[])
{
	LOAD_NIL(STACK(base, 0));
	LOAD_NIL(STACK(base, 1));
	LOAD_NIL(STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 5), 2);
		COPY(STACK(base, 3), CAR(lptr));
		COPY(STACK(base, 4), CDR(lptr));
		LOAD_CONS(lptr, &display[0][11]);
	}
	M127_1:;
	COPY(&display[0][3], STACK(base, 3));
	COPY(&display[0][9], STACK(base, 4));
	Fge(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		goto RETURN141;
	}
	COPY(&display[0][1], STACK(base, 3));
	COPY(&display[0][3], STACK(base, 4));
	Fchar(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 0));
	if(CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == '#')
	{
		COPY(&display[0][6], STACK(base, 3));
		Flength(STACK(base, 3));
		COPY(&display[0][11], STACK(base, 4));
		add_q(STACK(base, 3));
		COPY(&display[0][3], STACK(base, 3));
		F1plus(STACK(base, 3));
		COPY(STACK(base, 3), &display[0][3]);
	}
	else
	{
		if(CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == 'V')
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
			bool_result = CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == 'v';
		}
		if(bool_result)
		{
			Z99_get_arg(STACK(base, 3), display);
			COPY(&display[0][11], STACK(base, 4));
			add_q(STACK(base, 3));
			COPY(&display[0][3], STACK(base, 3));
			F1plus(STACK(base, 3));
			COPY(STACK(base, 3), &display[0][3]);
		}
		else
		{
			if(CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == ',')
			{
				LOAD_NIL(STACK(base, 3));
				COPY(&display[0][11], STACK(base, 4));
				add_q(STACK(base, 3));
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 3));
				Fdigit_char_p(STACK(base, 3), 1);
				COPY(STACK(base, 3), STACK(base, 1));
				if(CL_TRUEP(STACK(base, 1)))
				{
					M128_1:;
					COPY(&display[0][3], STACK(base, 3));
					F1plus(STACK(base, 3));
					COPY(STACK(base, 3), &display[0][3]);
					COPY(&display[0][9], STACK(base, 4));
					Fge(STACK(base, 3), 2);
					if(CL_TRUEP(STACK(base, 3)))
					{
						COPY(STACK(base, 1), STACK(base, 3));
						COPY(&display[0][11], STACK(base, 4));
						add_q(STACK(base, 3));
						goto RETURN142;
					}
					COPY(&display[0][1], STACK(base, 3));
					COPY(&display[0][3], STACK(base, 4));
					Fchar(STACK(base, 3));
					COPY(STACK(base, 3), STACK(base, 0));
					COPY(STACK(base, 0), STACK(base, 2));
					Fdigit_char_p(STACK(base, 2), 1);
					if(CL_TRUEP(STACK(base, 2)))
					{
					}
					else
					{
						COPY(STACK(base, 1), STACK(base, 3));
						COPY(&display[0][11], STACK(base, 4));
						add_q(STACK(base, 3));
						goto RETURN142;
					}
					LOAD_FIXNUM(10, STACK(base, 3));
					COPY(STACK(base, 1), STACK(base, 4));
					Fmult(STACK(base, 3), 2);
					COPY(STACK(base, 2), STACK(base, 4));
					Fplus(STACK(base, 3), 2);
					COPY(STACK(base, 3), STACK(base, 1));
					goto M128_1;
					RETURN142:;
				}
			}
		}
	}
	COPY(&display[0][3], STACK(base, 3));
	COPY(&display[0][9], STACK(base, 4));
	Flt(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(&display[0][1], STACK(base, 3));
		COPY(&display[0][3], STACK(base, 4));
		Fchar(STACK(base, 3));
		bool_result = CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == ',';
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(&display[0][3], STACK(base, 3));
		F1plus(STACK(base, 3));
		COPY(STACK(base, 3), &display[0][3]);
	}
	else
	{
		goto RETURN141;
	}
	goto M127_1;
	RETURN141:;
	COPY(&display[0][11], STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), &display[0][11]);
	COPY(&display[0][11], STACK(base, 0));
}

static void Z102_get_at_sign_or_colon(CL_FORM *base, CL_FORM *display[])
{
	LOAD_NIL(&display[0][13]);
	LOAD_NIL(&display[0][14]);
	M129_1:;
	COPY(&display[0][3], STACK(base, 0));
	COPY(&display[0][9], STACK(base, 1));
	Fge(STACK(base, 0), 2);
	if(CL_TRUEP(STACK(base, 0)))
	{
		LOAD_SMSTR((CL_FORM *)&Kformat2[2], STACK(base, 0));	/* Syntax error at end of control-string: ~s */
		COPY(&display[0][1], STACK(base, 1));
		Ferror(STACK(base, 0), 2);
	}
	else
	{
		COPY(&display[0][1], STACK(base, 0));
		COPY(&display[0][3], STACK(base, 1));
		Fchar(STACK(base, 0));
		if(CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == '@')
		{
			if(CL_TRUEP(&display[0][13]))
			{
				LOAD_SMSTR((CL_FORM *)&Kformat2[4], STACK(base, 0));	/* Too many at-signs supplied. */
				Ferror(STACK(base, 0), 1);
			}
			LOAD_SYMBOL(SYMBOL(Ssys, 1), &display[0][13]);	/* T */
			COPY(&display[0][3], STACK(base, 0));
			F1plus(STACK(base, 0));
			COPY(STACK(base, 0), &display[0][3]);
		}
		else
		{
			COPY(&display[0][1], STACK(base, 0));
			COPY(&display[0][3], STACK(base, 1));
			Fchar(STACK(base, 0));
			if(CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == ':')
			{
				if(CL_TRUEP(&display[0][14]))
				{
					LOAD_SMSTR((CL_FORM *)&Kformat2[6], STACK(base, 0));	/* Too many colons supplied. */
					Ferror(STACK(base, 0), 1);
				}
				LOAD_SYMBOL(SYMBOL(Ssys, 1), &display[0][14]);	/* T */
				COPY(&display[0][3], STACK(base, 0));
				F1plus(STACK(base, 0));
				COPY(STACK(base, 0), &display[0][3]);
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
				goto RETURN143;
			}
		}
	}
	goto M129_1;
	RETURN143:;
}

static void Z103_get_directive(CL_FORM *base, CL_FORM *display[])
{
	COPY(&display[0][3], STACK(base, 0));
	COPY(&display[0][9], STACK(base, 1));
	Fge(STACK(base, 0), 2);
	if(CL_TRUEP(STACK(base, 0)))
	{
		LOAD_SMSTR((CL_FORM *)&Kformat2[8], STACK(base, 0));	/* Syntax error at end of control-string: ~s */
		COPY(&display[0][1], STACK(base, 1));
		Ferror(STACK(base, 0), 2);
	}
	else
	{
		COPY(&display[0][1], STACK(base, 0));
		COPY(&display[0][3], STACK(base, 1));
		Fchar(STACK(base, 0));
		COPY(&display[0][3], STACK(base, 1));
		F1plus(STACK(base, 1));
		COPY(STACK(base, 1), &display[0][3]);
	}
}

static void Z104_skip(CL_FORM *base, CL_FORM *display[], int nargs)
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
	M130_1:;
	Z100_get_ctrl(STACK(base, 2), display);
	if(CL_CHARP(STACK(base, 2)) && GET_CHAR(STACK(base, 2)) == '~')
	{
		Z101_get_params(STACK(base, 3), display);
		Z102_get_at_sign_or_colon(STACK(base, 3), display);
		Z103_get_directive(STACK(base, 3), display);
		if(EQ(STACK(base, 3), STACK(base, 0)) || CL_FLOATP(STACK(base, 3)) && GET_FLOAT(STACK(base, 3)) == GET_FLOAT(STACK(base, 0)))
		{
			COPY(STACK(base, 3), STACK(base, 0));
			goto RETURN144;
		}
		else
		{
			if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '(')
			{
				LOAD_CHAR(')', STACK(base, 4));
				Z104_skip(STACK(base, 4), display, 1);
			}
			else
			{
				if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '[')
				{
					LOAD_CHAR(']', STACK(base, 4));
					Z104_skip(STACK(base, 4), display, 1);
				}
				else
				{
					if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '{')
					{
						LOAD_CHAR('}', STACK(base, 4));
						Z104_skip(STACK(base, 4), display, 1);
					}
					else
					{
						if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '<')
						{
							LOAD_CHAR('>', STACK(base, 4));
							Z104_skip(STACK(base, 4), display, 1);
						}
						else
						{
							if(CL_CHARP(STACK(base, 3)) && GET_CHAR(STACK(base, 3)) == '~')
							{
								if(CL_TRUEP(STACK(base, 1)))
								{
									COPY(STACK(base, 3), STACK(base, 0));
									goto RETURN144;
								}
							}
						}
					}
				}
			}
		}
	}
	goto M130_1;
	RETURN144:;
}

static void Z105_insert_space(CL_FORM *base, CL_FORM *display[])
{
	if(CL_NUMBERP(STACK(base, 0)))
	{
		COPY(&display[0][0], STACK(base, 2));
		stream_column(STACK(base, 2));
		Ffuncall(STACK(base, 2), 1);
		mv_count = 1;
		COPY(STACK(base, 1), STACK(base, 3));
		Fminus(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 1));
		M131_1:;
		COPY(STACK(base, 1), STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		Fge(STACK(base, 2), 2);
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN145;
		}
		LOAD_CHAR(' ', STACK(base, 2));
		COPY(&display[0][0], STACK(base, 3));
		Fwrite_char(STACK(base, 2), 2);
		mv_count = 1;
		F1plus(STACK(base, 1));
		goto M131_1;
		RETURN145:;
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}

static void Z106_padding(CL_FORM *base, CL_FORM *display[], int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_FIXNUM(0, STACK(base, 1));
		case 2:
		LOAD_FIXNUM(1, STACK(base, 2));
		case 3:
		LOAD_FIXNUM(0, STACK(base, 3));
		case 4:
		LOAD_CHAR(' ', STACK(base, 4));
		case 5:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	LOAD_FIXNUM(0, STACK(base, 5));
	M132_1:;
	COPY(STACK(base, 5), STACK(base, 6));
	COPY(STACK(base, 3), STACK(base, 7));
	Fge(STACK(base, 6), 2);
	if(CL_TRUEP(STACK(base, 6)))
	{
		goto RETURN146;
	}
	COPY(STACK(base, 4), STACK(base, 6));
	COPY(&display[0][0], STACK(base, 7));
	Fwrite_char(STACK(base, 6), 2);
	mv_count = 1;
	F1plus(STACK(base, 5));
	goto M132_1;
	RETURN146:;
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fplus(STACK(base, 5), 2);
	COPY(STACK(base, 5), STACK(base, 0));
	M133_1:;
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN147;
	}
	LOAD_FIXNUM(0, STACK(base, 5));
	M134_1:;
	COPY(STACK(base, 5), STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	Fge(STACK(base, 6), 2);
	if(CL_TRUEP(STACK(base, 6)))
	{
		goto RETURN148;
	}
	COPY(STACK(base, 4), STACK(base, 6));
	COPY(&display[0][0], STACK(base, 7));
	Fwrite_char(STACK(base, 6), 2);
	mv_count = 1;
	F1plus(STACK(base, 5));
	goto M134_1;
	RETURN148:;
	COPY(STACK(base, 0), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	Fplus(STACK(base, 5), 2);
	COPY(STACK(base, 5), STACK(base, 0));
	goto M133_1;
	RETURN147:;
}
