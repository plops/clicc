#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kmake_terminal_io)
{
	MAKE_STRING(25, "can\'t close *terminal-io*"),	/* 0 */
};

static void Z49_g156(CL_FORM *base);
static GLOBAL_FUNARG CZ49_g156 = {Z49_g156, 1};
static void Z50_g155(CL_FORM *base);
static void Z51_g153(CL_FORM *base);
static void Z52_g152(CL_FORM *base);
static void Z53_g151(CL_FORM *base);

void make_terminal_io(CL_FORM *base)
{
	c_stdin(STACK(base, 0));
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	c_stdout(STACK(base, 1));
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 2));
	GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 3));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 101), STACK(base, 4));	/* FILE-IO */
	LOAD_SYMBOL(SYMBOL(Slisp, 81), STACK(base, 5));	/* READC */
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z53_g151, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 82), STACK(base, 7));	/* UNREADC */
	{
		GEN_CLOSURE(array, STACK(base, 8), 4, Z52_g152, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 8));
	}
	COPY(STACK(base, 8), STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 83), STACK(base, 9));	/* WRITEC */
	{
		GEN_CLOSURE(array, STACK(base, 10), 5, Z51_g153, 1);
		COPY(STACK(base, 1), &array[3]);
		COPY(STACK(base, 2), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 10));
	}
	COPY(STACK(base, 10), STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Slisp, 84), STACK(base, 11));	/* COLUMN */
	{
		GEN_CLOSURE(array, STACK(base, 12), 4, Z50_g155, 0);
		COPY(STACK(base, 2), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 12));
	}
	COPY(STACK(base, 12), STACK(base, 12));
	LOAD_SYMBOL(SYMBOL(Slisp, 88), STACK(base, 13));	/* CLOSE */
	LOAD_GLOBFUN(&CZ49_g156, STACK(base, 14));
	make_stream(STACK(base, 3), 12);
	COPY(STACK(base, 3), STACK(base, 0));
}

static void Z49_g156(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&Kmake_terminal_io[0], STACK(base, 0));	/* can't close *terminal-io* */
}

static void Z50_g155(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
}

static void Z51_g153(CL_FORM *base)
{
	if(CL_CHARP(STACK(base, 1)) && GET_CHAR(STACK(base, 1)) == '\n')
	{
		LOAD_FIXNUM(0, INDIRECT(GET_FORM(STACK(base, 0)) + 4));
	}
	else
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
		F1plus(STACK(base, 2));
		COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 4));
	}
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	c_fputc(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z52_g152(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	c_ungetc(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z53_g151(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_fgetc(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}
