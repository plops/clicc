#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFprobe_file)
{
	MAKE_STRING(1, "/"),	/* 0 */
	MAKE_STRING(1, "/"),	/* 2 */
	MAKE_STRING(25, "Error reading link ~S: ~S"),	/* 4 */
	MAKE_STRING(18, "Unknown error [~d]"),	/* 6 */
	MAKE_STRING(4, "/../"),	/* 8 */
	MAKE_STRING(2, "./"),	/* 10 */
	MAKE_STRING(2, "./"),	/* 12 */
};

void Fprobe_file(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
	unix_namestring(STACK(base, 1), 2);
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 2));
		unix_file_kind(STACK(base, 2), 1);
		bool_result = CL_TRUEP(STACK(base, 2));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(STACK(base, 1), STACK(base, 2));
		Flength(STACK(base, 2));
		Fplusp(STACK(base, 2));
		if(CL_TRUEP(STACK(base, 2)))
		{
			COPY(STACK(base, 1), STACK(base, 2));
			LOAD_FIXNUM(0, STACK(base, 3));
			Fschar(STACK(base, 2));
			LOAD_CHAR('/', STACK(base, 3));
			FcharE(STACK(base, 2), 2);
			bool_result = CL_TRUEP(STACK(base, 2));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 1), STACK(base, 2));
		}
		else
		{
			unix_current_directory(STACK(base, 2));
			COPY(&mv_buf[0], STACK(base, 3));
			{
				int nargs;
				nargs = 2;
				mv_count = 1;
				switch(nargs)
				{
					case 0:
					LOAD_NIL(STACK(base, 2));
					case 1:
					LOAD_NIL(STACK(base, 3));
					nargs = 2;
				}
				Flist(STACK(base, 4), nargs - 2);
				if(CL_TRUEP(STACK(base, 2)))
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 2));	/* SIMPLE-STRING */
					LOAD_SMSTR((CL_FORM *)&KFprobe_file[0], STACK(base, 4));	/* / */
					COPY(STACK(base, 1), STACK(base, 5));
					Fconcatenate(STACK(base, 2), 4);
				}
				else
				{
					COPY(STACK(base, 1), STACK(base, 2));
				}
			}
		}
		COPY(STACK(base, 2), STACK(base, 3));
		Flength(STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 5));
		Fzerop(STACK(base, 5));
		if(CL_TRUEP(STACK(base, 5)))
		{
		}
		else
		{
			LOAD_FIXNUM(1024, STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 6));	/* INITIAL-ELEMENT */
			LOAD_FIXNUM(0, STACK(base, 7));
			Fcode_char(STACK(base, 7));
			Fmake_string(STACK(base, 5), 3);
			LOAD_FIXNUM(0, STACK(base, 6));
			LOAD_FIXNUM(0, STACK(base, 7));
			M36_1:;
			LOAD_CHAR('/', STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 9));
			LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 10));	/* START */
			COPY(STACK(base, 7), STACK(base, 11));
			Fposition(STACK(base, 8), 4);
			if(CL_TRUEP(STACK(base, 8)))
			{
			}
			else
			{
				COPY(STACK(base, 3), STACK(base, 8));
			}
			COPY(STACK(base, 6), STACK(base, 9));
			COPY(STACK(base, 8), STACK(base, 10));
			COPY(STACK(base, 7), STACK(base, 11));
			Fminus(STACK(base, 10), 2);
			Fplus(STACK(base, 9), 2);
			COPY(STACK(base, 5), STACK(base, 10));
			COPY(STACK(base, 4), STACK(base, 11));
			LOAD_SYMBOL(SYMBOL(Slisp, 237), STACK(base, 12));	/* START1 */
			COPY(STACK(base, 6), STACK(base, 13));
			LOAD_SYMBOL(SYMBOL(Slisp, 238), STACK(base, 14));	/* END1 */
			COPY(STACK(base, 9), STACK(base, 15));
			LOAD_SYMBOL(SYMBOL(Slisp, 211), STACK(base, 16));	/* START2 */
			COPY(STACK(base, 7), STACK(base, 17));
			LOAD_SYMBOL(SYMBOL(Slisp, 212), STACK(base, 18));	/* END2 */
			COPY(STACK(base, 8), STACK(base, 19));
			Freplace(STACK(base, 10), 10);
			COPY(STACK(base, 8), STACK(base, 10));
			Fzerop(STACK(base, 10));
			if(CL_TRUEP(STACK(base, 10)))
			{
				LOAD_SMSTR((CL_FORM *)&KFprobe_file[2], STACK(base, 10));	/* / */
			}
			else
			{
				COPY(STACK(base, 5), STACK(base, 10));
			}
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 11));	/* T */
			unix_file_kind(STACK(base, 10), 2);
			if(CL_TRUEP(STACK(base, 10)))
			{
			}
			else
			{
				LOAD_NIL(STACK(base, 2));
				goto RETURN37;
			}
			if(CL_SYMBOLP(STACK(base, 10)) && GET_SYMBOL(STACK(base, 10)) == SYMBOL(Slisp, 235))	/* LINK */
			{
				COPY(STACK(base, 5), STACK(base, 11));
				unix_readlink(STACK(base, 11));
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
						nargs = 2;
					}
					Flist(STACK(base, 13), nargs - 2);
					if(CL_TRUEP(STACK(base, 11)))
					{
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&KFprobe_file[4], STACK(base, 14));	/* Error reading link ~S: ~S */
						COPY(STACK(base, 5), STACK(base, 15));
						LOAD_FIXNUM(0, STACK(base, 16));
						COPY(STACK(base, 6), STACK(base, 17));
						Fsubseq(STACK(base, 15), 3);
						COPY(SYMVAL(Slisp, 224), STACK(base, 16));	/* *UNIX-ERRORS* */
						COPY(STACK(base, 12), STACK(base, 17));
						Farray_in_bounds_p(STACK(base, 16), 2);
						if(CL_TRUEP(STACK(base, 16)))
						{
							COPY(SYMVAL(Slisp, 224), STACK(base, 16));	/* *UNIX-ERRORS* */
							COPY(STACK(base, 12), STACK(base, 17));
							Fsvref(STACK(base, 16));
						}
						else
						{
							LOAD_NIL(STACK(base, 16));
							LOAD_SMSTR((CL_FORM *)&KFprobe_file[6], STACK(base, 17));	/* Unknown error [~d] */
							COPY(STACK(base, 12), STACK(base, 18));
							Fformat(STACK(base, 16), 3);
						}
						Ferror(STACK(base, 14), 3);
					}
					COPY(STACK(base, 11), STACK(base, 14));
					Flength(STACK(base, 14));
					Fzerop(STACK(base, 14));
					if(CL_TRUEP(STACK(base, 14)))
					{
						bool_result = TRUE;
					}
					else
					{
						COPY(STACK(base, 11), STACK(base, 15));
						LOAD_FIXNUM(0, STACK(base, 16));
						Fschar(STACK(base, 15));
						LOAD_CHAR('/', STACK(base, 16));
						FcharNE(STACK(base, 15), 2);
						bool_result = CL_TRUEP(STACK(base, 15));
					}
					if(bool_result)
					{
						COPY(STACK(base, 5), STACK(base, 14));
						LOAD_FIXNUM(0, STACK(base, 15));
						Fcode_char(STACK(base, 15));
						LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 16));	/* START */
						COPY(STACK(base, 6), STACK(base, 17));
						LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 18));	/* END */
						COPY(STACK(base, 9), STACK(base, 19));
						Ffill(STACK(base, 14), 6);
					}
					else
					{
						COPY(STACK(base, 5), STACK(base, 14));
						LOAD_SMSTR((CL_FORM *)&KFprobe_file[8], STACK(base, 15));	/* /../ */
						LOAD_SYMBOL(SYMBOL(Slisp, 238), STACK(base, 16));	/* END1 */
						LOAD_FIXNUM(4, STACK(base, 17));
						FstringE(STACK(base, 14), 4);
						if(CL_TRUEP(STACK(base, 14)))
						{
							LOAD_CHAR('/', STACK(base, 14));
							COPY(STACK(base, 5), STACK(base, 15));
							LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 16));	/* START */
							LOAD_FIXNUM(4, STACK(base, 17));
							Fposition(STACK(base, 14), 4);
							if(CL_TRUEP(STACK(base, 14)))
							{
							}
							else
							{
								LOAD_FIXNUM(0, STACK(base, 14));
							}
							COPY(STACK(base, 5), STACK(base, 15));
							LOAD_FIXNUM(0, STACK(base, 16));
							Fcode_char(STACK(base, 16));
							LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 17));	/* START */
							COPY(STACK(base, 14), STACK(base, 18));
							LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 19));	/* END */
							COPY(STACK(base, 9), STACK(base, 20));
							Ffill(STACK(base, 15), 6);
							COPY(STACK(base, 14), STACK(base, 6));
						}
						else
						{
							COPY(STACK(base, 5), STACK(base, 14));
							LOAD_FIXNUM(0, STACK(base, 15));
							Fcode_char(STACK(base, 15));
							LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 16));	/* END */
							COPY(STACK(base, 9), STACK(base, 17));
							Ffill(STACK(base, 14), 4);
							LOAD_FIXNUM(0, STACK(base, 6));
						}
					}
					COPY(STACK(base, 8), STACK(base, 14));
					COPY(STACK(base, 3), STACK(base, 15));
					Fnumeql(STACK(base, 14), 2);
					if(CL_TRUEP(STACK(base, 14)))
					{
						COPY(STACK(base, 11), STACK(base, 4));
					}
					else
					{
						LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 14));	/* SIMPLE-STRING */
						COPY(STACK(base, 11), STACK(base, 15));
						COPY(STACK(base, 4), STACK(base, 16));
						COPY(STACK(base, 8), STACK(base, 17));
						Fsubseq(STACK(base, 16), 2);
						Fconcatenate(STACK(base, 14), 3);
						COPY(STACK(base, 14), STACK(base, 4));
					}
					COPY(STACK(base, 4), STACK(base, 14));
					Flength(STACK(base, 14));
					COPY(STACK(base, 14), STACK(base, 3));
					LOAD_FIXNUM(0, STACK(base, 7));
				}
			}
			else
			{
				COPY(STACK(base, 8), STACK(base, 11));
				COPY(STACK(base, 3), STACK(base, 12));
				Fnumeql(STACK(base, 11), 2);
				if(CL_TRUEP(STACK(base, 11)))
				{
					COPY(STACK(base, 5), STACK(base, 2));
					LOAD_FIXNUM(0, STACK(base, 3));
					COPY(STACK(base, 9), STACK(base, 4));
					Fsubseq(STACK(base, 2), 3);
					goto RETURN37;
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 10)) && GET_SYMBOL(STACK(base, 10)) == SYMBOL(Slisp, 184))	/* DIRECTORY */
					{
						LOAD_CHAR('/', STACK(base, 11));
						COPY(STACK(base, 5), STACK(base, 12));
						COPY(STACK(base, 9), STACK(base, 13));
						Fset_schar(STACK(base, 11));
						COPY(STACK(base, 9), STACK(base, 11));
						F1plus(STACK(base, 11));
						COPY(STACK(base, 11), STACK(base, 6));
						COPY(STACK(base, 8), STACK(base, 11));
						F1plus(STACK(base, 11));
						COPY(STACK(base, 11), STACK(base, 7));
					}
					else
					{
						LOAD_NIL(STACK(base, 2));
						goto RETURN37;
					}
				}
			}
			goto M36_1;
			RETURN37:;
		}
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
			BIND_SPECIAL(SYMBOL(Slisp, 218), STACK(base, 3));	/* *IGNORE-WILDCARDS* */
			COPY(STACK(base, 2), STACK(base, 4));
			Flength(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 5));
			Fmake_string(STACK(base, 5), 1);
			LOAD_FIXNUM(0, STACK(base, 6));
			LOAD_FIXNUM(0, STACK(base, 7));
			LOAD_NIL(STACK(base, 8));
			LOAD_FIXNUM(0, STACK(base, 9));
			M37_1:;
			COPY(STACK(base, 9), STACK(base, 10));
			COPY(STACK(base, 4), STACK(base, 11));
			Fge(STACK(base, 10), 2);
			if(CL_TRUEP(STACK(base, 10)))
			{
				goto RETURN39;
			}
			COPY(STACK(base, 2), STACK(base, 10));
			COPY(STACK(base, 9), STACK(base, 11));
			Fschar(STACK(base, 10));
			COPY(STACK(base, 10), STACK(base, 11));
			LOAD_CHAR('.', STACK(base, 12));
			FcharE(STACK(base, 11), 2);
			if(CL_TRUEP(STACK(base, 11)))
			{
				if(CL_TRUEP(STACK(base, 7)))
				{
					COPY(STACK(base, 7), STACK(base, 11));
					F1plus(STACK(base, 11));
					COPY(STACK(base, 11), STACK(base, 7));
				}
				COPY(STACK(base, 10), STACK(base, 11));
				COPY(STACK(base, 5), STACK(base, 12));
				COPY(STACK(base, 6), STACK(base, 13));
				Fset_schar(STACK(base, 11));
				COPY(STACK(base, 6), STACK(base, 11));
				F1plus(STACK(base, 11));
				COPY(STACK(base, 11), STACK(base, 6));
			}
			else
			{
				COPY(STACK(base, 10), STACK(base, 11));
				LOAD_CHAR('/', STACK(base, 12));
				FcharE(STACK(base, 11), 2);
				if(CL_TRUEP(STACK(base, 11)))
				{
					if(CL_FIXNUMP(STACK(base, 7)) && GET_FIXNUM(STACK(base, 7)) == 0)
					{
						if(CL_TRUEP(STACK(base, 8)))
						{
						}
						else
						{
							COPY(STACK(base, 6), STACK(base, 8));
							COPY(STACK(base, 10), STACK(base, 11));
							COPY(STACK(base, 5), STACK(base, 12));
							COPY(STACK(base, 6), STACK(base, 13));
							Fset_schar(STACK(base, 11));
							COPY(STACK(base, 6), STACK(base, 11));
							F1plus(STACK(base, 11));
							COPY(STACK(base, 11), STACK(base, 6));
						}
					}
					else
					{
						if(CL_FIXNUMP(STACK(base, 7)) && GET_FIXNUM(STACK(base, 7)) == 1)
						{
							COPY(STACK(base, 6), STACK(base, 11));
							F1minus(STACK(base, 11));
							COPY(STACK(base, 11), STACK(base, 6));
						}
						else
						{
							if(CL_FIXNUMP(STACK(base, 7)) && GET_FIXNUM(STACK(base, 7)) == 2)
							{
								if(CL_TRUEP(STACK(base, 8)))
								{
									COPY(STACK(base, 8), STACK(base, 11));
									Fzerop(STACK(base, 11));
									bool_result = NOT(CL_TRUEP(STACK(base, 11)));
								}
								else
								{
									bool_result = FALSE;
								}
								if(bool_result)
								{
									LOAD_CHAR('/', STACK(base, 11));
									COPY(STACK(base, 5), STACK(base, 12));
									LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 13));	/* END */
									COPY(STACK(base, 8), STACK(base, 14));
									LOAD_SYMBOL(SYMBOL(Slisp, 229), STACK(base, 15));	/* FROM-END */
									LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 16));	/* T */
									Fposition(STACK(base, 11), 6);
									if(CL_TRUEP(STACK(base, 11)))
									{
										COPY(STACK(base, 11), STACK(base, 12));
									}
									else
									{
										LOAD_FIXNUM(0, STACK(base, 12));
									}
									LOAD_FIXNUM(2, STACK(base, 13));
									Fplus(STACK(base, 12), 2);
									COPY(STACK(base, 8), STACK(base, 13));
									Fnumeql(STACK(base, 12), 2);
									if(CL_TRUEP(STACK(base, 12)))
									{
										COPY(STACK(base, 5), STACK(base, 12));
										COPY(STACK(base, 8), STACK(base, 13));
										LOAD_FIXNUM(2, STACK(base, 14));
										Fminus(STACK(base, 13), 2);
										Fschar(STACK(base, 12));
										LOAD_CHAR('.', STACK(base, 13));
										FcharE(STACK(base, 12), 2);
										if(CL_TRUEP(STACK(base, 12)))
										{
											COPY(STACK(base, 5), STACK(base, 12));
											COPY(STACK(base, 8), STACK(base, 13));
											F1minus(STACK(base, 13));
											Fschar(STACK(base, 12));
											LOAD_CHAR('.', STACK(base, 13));
											FcharE(STACK(base, 12), 2);
											bool_result = CL_TRUEP(STACK(base, 12));
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
										COPY(STACK(base, 10), STACK(base, 12));
										COPY(STACK(base, 5), STACK(base, 13));
										COPY(STACK(base, 6), STACK(base, 14));
										Fset_schar(STACK(base, 12));
										COPY(STACK(base, 6), STACK(base, 12));
										F1plus(STACK(base, 12));
										COPY(STACK(base, 12), STACK(base, 6));
										COPY(STACK(base, 6), STACK(base, 8));
									}
									else
									{
										if(CL_TRUEP(STACK(base, 11)))
										{
											COPY(STACK(base, 11), STACK(base, 12));
											F1plus(STACK(base, 12));
											COPY(STACK(base, 12), STACK(base, 6));
										}
										else
										{
											LOAD_FIXNUM(0, STACK(base, 6));
										}
										COPY(STACK(base, 11), STACK(base, 8));
									}
								}
								else
								{
									COPY(STACK(base, 6), STACK(base, 8));
									COPY(STACK(base, 10), STACK(base, 11));
									COPY(STACK(base, 5), STACK(base, 12));
									COPY(STACK(base, 6), STACK(base, 13));
									Fset_schar(STACK(base, 11));
									COPY(STACK(base, 6), STACK(base, 11));
									F1plus(STACK(base, 11));
									COPY(STACK(base, 11), STACK(base, 6));
								}
							}
							else
							{
								COPY(STACK(base, 6), STACK(base, 8));
								COPY(STACK(base, 10), STACK(base, 11));
								COPY(STACK(base, 5), STACK(base, 12));
								COPY(STACK(base, 6), STACK(base, 13));
								Fset_schar(STACK(base, 11));
								COPY(STACK(base, 6), STACK(base, 11));
								F1plus(STACK(base, 11));
								COPY(STACK(base, 11), STACK(base, 6));
							}
						}
					}
					LOAD_FIXNUM(0, STACK(base, 7));
				}
				else
				{
					LOAD_NIL(STACK(base, 7));
					COPY(STACK(base, 10), STACK(base, 11));
					COPY(STACK(base, 5), STACK(base, 12));
					COPY(STACK(base, 6), STACK(base, 13));
					Fset_schar(STACK(base, 11));
					COPY(STACK(base, 6), STACK(base, 11));
					F1plus(STACK(base, 11));
					COPY(STACK(base, 11), STACK(base, 6));
				}
			}
			F1plus(STACK(base, 9));
			goto M37_1;
			RETURN39:;
			if(CL_TRUEP(STACK(base, 8)))
			{
				COPY(STACK(base, 8), STACK(base, 9));
				Fzerop(STACK(base, 9));
				bool_result = NOT(CL_TRUEP(STACK(base, 9)));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				if(CL_FIXNUMP(STACK(base, 7)) && GET_FIXNUM(STACK(base, 7)) == 1)
				{
					COPY(STACK(base, 6), STACK(base, 9));
					F1minus(STACK(base, 9));
					COPY(STACK(base, 9), STACK(base, 6));
				}
				else
				{
					if(CL_FIXNUMP(STACK(base, 7)) && GET_FIXNUM(STACK(base, 7)) == 2)
					{
						COPY(STACK(base, 8), STACK(base, 9));
						LOAD_FIXNUM(2, STACK(base, 10));
						Fge(STACK(base, 9), 2);
						if(CL_TRUEP(STACK(base, 9)))
						{
							COPY(STACK(base, 5), STACK(base, 9));
							COPY(STACK(base, 8), STACK(base, 10));
							F1minus(STACK(base, 10));
							Fschar(STACK(base, 9));
							LOAD_CHAR('.', STACK(base, 10));
							FcharE(STACK(base, 9), 2);
							if(CL_TRUEP(STACK(base, 9)))
							{
								COPY(STACK(base, 5), STACK(base, 9));
								COPY(STACK(base, 8), STACK(base, 10));
								LOAD_FIXNUM(2, STACK(base, 11));
								Fminus(STACK(base, 10), 2);
								Fschar(STACK(base, 9));
								LOAD_CHAR('.', STACK(base, 10));
								FcharE(STACK(base, 9), 2);
								if(CL_TRUEP(STACK(base, 9)))
								{
									COPY(STACK(base, 8), STACK(base, 9));
									LOAD_FIXNUM(2, STACK(base, 10));
									Fnumeql(STACK(base, 9), 2);
									if(CL_TRUEP(STACK(base, 9)))
									{
										bool_result = TRUE;
									}
									else
									{
										COPY(STACK(base, 5), STACK(base, 10));
										COPY(STACK(base, 8), STACK(base, 11));
										LOAD_FIXNUM(3, STACK(base, 12));
										Fminus(STACK(base, 11), 2);
										Fschar(STACK(base, 10));
										LOAD_CHAR('/', STACK(base, 11));
										FcharE(STACK(base, 10), 2);
										bool_result = CL_TRUEP(STACK(base, 10));
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
						}
						else
						{
							bool_result = FALSE;
						}
						if(bool_result)
						{
						}
						else
						{
							LOAD_CHAR('/', STACK(base, 9));
							COPY(STACK(base, 5), STACK(base, 10));
							LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 11));	/* END */
							COPY(STACK(base, 8), STACK(base, 12));
							LOAD_SYMBOL(SYMBOL(Slisp, 229), STACK(base, 13));	/* FROM-END */
							LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 14));	/* T */
							Fposition(STACK(base, 9), 6);
							if(CL_TRUEP(STACK(base, 9)))
							{
								COPY(STACK(base, 9), STACK(base, 10));
								F1plus(STACK(base, 10));
								COPY(STACK(base, 10), STACK(base, 6));
							}
							else
							{
								LOAD_SMSTR((CL_FORM *)&KFprobe_file[10], STACK(base, 4));	/* ./ */
								goto RETURN38;
							}
						}
					}
				}
			}
			COPY(STACK(base, 6), STACK(base, 9));
			Fzerop(STACK(base, 9));
			if(CL_TRUEP(STACK(base, 9)))
			{
				LOAD_SMSTR((CL_FORM *)&KFprobe_file[12], STACK(base, 4));	/* ./ */
			}
			else
			{
				COPY(STACK(base, 6), STACK(base, 9));
				COPY(STACK(base, 4), STACK(base, 10));
				Fnumeql(STACK(base, 9), 2);
				if(CL_TRUEP(STACK(base, 9)))
				{
					COPY(STACK(base, 5), STACK(base, 4));
				}
				else
				{
					COPY(STACK(base, 5), STACK(base, 4));
					LOAD_FIXNUM(0, STACK(base, 5));
					Fsubseq(STACK(base, 4), 3);
				}
			}
			RETURN38:;
			Fpathname(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 0));
			RESTORE_SPECIAL;
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
