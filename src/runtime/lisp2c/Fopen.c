#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFopen)
{
	MAKE_STRING(17, "illegal option ~s"),	/* 0 */
	MAKE_STRING(1, "r"),	/* 2 */
	MAKE_STRING(17, "~s does not exist"),	/* 4 */
	MAKE_STRING(2, "w+"),	/* 6 */
	MAKE_STRING(17, "can not create ~S"),	/* 8 */
	MAKE_STRING(17, "illegal option ~s"),	/* 10 */
	MAKE_STRING(22, "file already exists ~s"),	/* 12 */
	MAKE_STRING(2, "r+"),	/* 14 */
	MAKE_STRING(2, "a+"),	/* 16 */
	MAKE_STRING(2, "w+"),	/* 18 */
	MAKE_STRING(23, "unimplemented option ~S"),	/* 20 */
	MAKE_STRING(2, "r+"),	/* 22 */
	MAKE_STRING(15, "can not open ~S"),	/* 24 */
};

static void Z57_get_io_file(CL_FORM *base, CL_FORM *display[]);

void Fopen(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	BOOL supl_flags[3];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 149),	/* DIRECTION */
		SYMBOL(Slisp, 151),	/* IF-EXISTS */
		SYMBOL(Slisp, 158),	/* IF-DOES-NOT-EXIST */
	};
	keysort(STACK(base, 1), nargs - 1, 3, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 148), STACK(base, 1));	/* INPUT */
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 150), STACK(base, 2));	/* ERROR */
	}
	if(NOT(supl_flags[2]))
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 148))	/* INPUT */
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 150), STACK(base, 3));	/* ERROR */
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 152))	/* OUTPUT */
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
				bool_result = CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 153);	/* IO */
			}
			if(bool_result)
			{
				if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 154))	/* OVERWRITE */
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
					bool_result = CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 155);	/* APPEND */
				}
				if(bool_result)
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 150), STACK(base, 3));	/* ERROR */
				}
				else
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 156), STACK(base, 3));	/* CREATE */
				}
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 157))	/* PROBE */
				{
					LOAD_NIL(STACK(base, 3));
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFopen[0], STACK(base, 4));	/* illegal option ~s */
					COPY(STACK(base, 1), STACK(base, 5));
					Ferror(STACK(base, 4), 2);
				}
			}
		}
	}
	{
		CONTENV new_cont;
		CL_FORM *caller_base;
		new_cont.bind_top = bind_top;
		new_cont.last = last_cont;
		LOAD_UNIQUE_TAG(STACK(base, 4));
		caller_base = (CL_FORM *)SETJMP(new_cont.jmp_buf);
		if(caller_base == NULL)
		{
			last_cont = &new_cont;
			LOAD_NIL(STACK(base, 5));
			LOAD_NIL(STACK(base, 6));
			COPY(STACK(base, 0), STACK(base, 7));
			Fnamestring(STACK(base, 7));
			mv_count = 1;
			COPY(STACK(base, 7), STACK(base, 0));
			LOAD_SMSTR((CL_FORM *)&KFopen[2], STACK(base, 8));	/* r */
			c_fopen(STACK(base, 7));
			COPY(STACK(base, 7), STACK(base, 5));
			if(CL_TRUEP(STACK(base, 5)))
			{
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 150))	/* ERROR */
				{
					LOAD_SMSTR((CL_FORM *)&KFopen[4], STACK(base, 7));	/* ~s does not exist */
					COPY(STACK(base, 0), STACK(base, 8));
					Ferror(STACK(base, 7), 2);
				}
				else
				{
					if(CL_TRUEP(STACK(base, 3)))
					{
						if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 156))	/* CREATE */
						{
							COPY(STACK(base, 0), STACK(base, 7));
							LOAD_SMSTR((CL_FORM *)&KFopen[6], STACK(base, 8));	/* w+ */
							c_fopen(STACK(base, 7));
							COPY(STACK(base, 7), STACK(base, 6));
							if(CL_TRUEP(STACK(base, 6)))
							{
							}
							else
							{
								LOAD_SMSTR((CL_FORM *)&KFopen[8], STACK(base, 7));	/* can not create ~S */
								COPY(STACK(base, 0), STACK(base, 8));
								Ferror(STACK(base, 7), 2);
							}
						}
						else
						{
							LOAD_SMSTR((CL_FORM *)&KFopen[10], STACK(base, 7));	/* illegal option ~s */
							COPY(STACK(base, 3), STACK(base, 8));
							Ferror(STACK(base, 7), 2);
						}
					}
					else
					{
						LOAD_NIL(STACK(base, 0));
						goto RETURN14;
					}
				}
			}
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 148))	/* INPUT */
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
				bool_result = CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 157);	/* PROBE */
			}
			if(bool_result)
			{
				if(CL_TRUEP(STACK(base, 6)))
				{
					COPY(STACK(base, 6), STACK(base, 7));
					stream_close(STACK(base, 7));
					COPY(STACK(base, 6), STACK(base, 8));
					Ffuncall(STACK(base, 7), 2);
					COPY(STACK(base, 0), STACK(base, 7));
					LOAD_SMSTR((CL_FORM *)&KFopen[22], STACK(base, 8));	/* r+ */
					c_fopen(STACK(base, 7));
					COPY(STACK(base, 7), STACK(base, 5));
				}
				if(CL_TRUEP(STACK(base, 5)))
				{
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&KFopen[24], STACK(base, 7));	/* can not open ~S */
					COPY(STACK(base, 0), STACK(base, 8));
					Ferror(STACK(base, 7), 2);
				}
				COPY(STACK(base, 5), STACK(base, 7));
				COPY(STACK(base, 0), STACK(base, 8));
				make_file_input_stream(STACK(base, 7));
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 157))	/* PROBE */
				{
					COPY(STACK(base, 7), STACK(base, 8));
					stream_close(STACK(base, 8));
					COPY(STACK(base, 7), STACK(base, 9));
					Ffuncall(STACK(base, 8), 2);
				}
				COPY(STACK(base, 7), STACK(base, 0));
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 152))	/* OUTPUT */
				{
					display[0] = STACK(base, 0);
					Z57_get_io_file(STACK(base, 7), display);
					COPY(STACK(base, 0), STACK(base, 8));
					make_file_output_stream(STACK(base, 7));
					COPY(STACK(base, 7), STACK(base, 0));
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 153))	/* IO */
					{
						display[0] = STACK(base, 0);
						Z57_get_io_file(STACK(base, 7), display);
						COPY(STACK(base, 0), STACK(base, 8));
						make_file_io_stream(STACK(base, 7));
						COPY(STACK(base, 7), STACK(base, 0));
					}
					else
					{
						LOAD_NIL(STACK(base, 0));
					}
				}
			}
			RETURN14:;
			last_cont = new_cont.last;
		}
		else
		{
			last_cont = new_cont.last;
			if(EQ(STACK(caller_base, 0), STACK(base, 4)))
			{
				COPY(STACK(caller_base, 1), STACK(base, 0));
			}
			else
			{
				call_cont(caller_base);
			}
		}
	}
}

static void Z57_get_io_file(CL_FORM *base, CL_FORM *display[])
{
	if(CL_TRUEP(&display[0][5]))
	{
		COPY(&display[0][5], STACK(base, 0));
		c_fclose(STACK(base, 0));
		if(CL_SYMBOLP(&display[0][2]) && GET_SYMBOL(&display[0][2]) == SYMBOL(Slisp, 150))	/* ERROR */
		{
			LOAD_SMSTR((CL_FORM *)&KFopen[12], STACK(base, 0));	/* file already exists ~s */
			COPY(&display[0][0], STACK(base, 1));
			Ferror(STACK(base, 0), 2);
		}
		else
		{
			if(CL_SYMBOLP(&display[0][2]) && GET_SYMBOL(&display[0][2]) == SYMBOL(Slisp, 154))	/* OVERWRITE */
			{
				COPY(&display[0][0], STACK(base, 0));
				LOAD_SMSTR((CL_FORM *)&KFopen[14], STACK(base, 1));	/* r+ */
				c_fopen(STACK(base, 0));
			}
			else
			{
				if(CL_SYMBOLP(&display[0][2]) && GET_SYMBOL(&display[0][2]) == SYMBOL(Slisp, 155))	/* APPEND */
				{
					COPY(&display[0][0], STACK(base, 0));
					LOAD_SMSTR((CL_FORM *)&KFopen[16], STACK(base, 1));	/* a+ */
					c_fopen(STACK(base, 0));
				}
				else
				{
					if(CL_SYMBOLP(&display[0][2]) && GET_SYMBOL(&display[0][2]) == SYMBOL(Slisp, 159))	/* SUPERSEDE */
					{
						LOAD_T(STACK(base, 0));
					}
					else
					{
						LOAD_NIL(STACK(base, 0));
					}
					if(CL_TRUEP(STACK(base, 0)))
					{
						bool_result = TRUE;
					}
					else
					{
						bool_result = CL_SYMBOLP(&display[0][2]) && GET_SYMBOL(&display[0][2]) == SYMBOL(Slisp, 160);	/* NEW-VERSION */
					}
					if(bool_result)
					{
						COPY(&display[0][0], STACK(base, 0));
						LOAD_SMSTR((CL_FORM *)&KFopen[18], STACK(base, 1));	/* w+ */
						c_fopen(STACK(base, 0));
					}
					else
					{
						if(CL_TRUEP(&display[0][2]))
						{
							LOAD_SMSTR((CL_FORM *)&KFopen[20], STACK(base, 0));	/* unimplemented option ~S */
							COPY(&display[0][2], STACK(base, 1));
							Ferror(STACK(base, 0), 2);
						}
						else
						{
							COPY(&display[0][4], STACK(base, 0));
							LOAD_NIL(STACK(base, 1));
							call_cont(STACK(base, 0));
						}
					}
				}
			}
		}
	}
	else
	{
		COPY(&display[0][6], STACK(base, 0));
	}
}
