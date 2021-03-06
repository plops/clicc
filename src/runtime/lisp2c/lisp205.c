/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

extern CL_INIT XImake_terminal_io[];
extern CL_INIT XFmake_terminal_io[];
CL_INIT Kmake_terminal_io[] =
{
	MAKE_STRING(14, "stdin / stdout"),	/* 0 */
	MAKE_STRING(25, "can\'t close *terminal-io*"),	/* 2 */
};

static void Z115_lambda(CL_FORM *base);
GEN_STATIC_GLOBAL_FUNARG(CZ115_lambda, Z115_lambda, 1);
static void Z116_lambda(CL_FORM *base);
static void Z117_lambda(CL_FORM *base);
static void Z118_lambda(CL_FORM *base);
static void Z119_lambda(CL_FORM *base);

void make_terminal_io(CL_FORM *base)
{
	c_stdin(ARG(0));
	GEN_HEAPVAR(ARG(0), ARG(1));
	c_stdout(ARG(1));
	GEN_HEAPVAR(ARG(1), ARG(2));
	LOAD_SMALLFIXNUM(0, ARG(2));
	GEN_HEAPVAR(ARG(2), ARG(3));
	LOAD_SYMBOL(SYMBOL(Slisp, 92), ARG(3));	/* FILE-IO */
	LOAD_SMSTR((CL_FORM *)&Kmake_terminal_io[0], ARG(4));	/* stdin / stdout */
	{
		GEN_CLOSURE(array, ARG(5), 4, Z119_lambda, 0);
		COPY(ARG(0), &array[3]);
		LOAD_CLOSURE(array, ARG(5));
	}
	COPY(ARG(5), ARG(5));
	{
		GEN_CLOSURE(array, ARG(6), 4, Z118_lambda, 1);
		COPY(ARG(0), &array[3]);
		LOAD_CLOSURE(array, ARG(6));
	}
	COPY(ARG(6), ARG(6));
	{
		GEN_CLOSURE(array, ARG(7), 5, Z117_lambda, 1);
		COPY(ARG(1), &array[3]);
		COPY(ARG(2), &array[4]);
		LOAD_CLOSURE(array, ARG(7));
	}
	COPY(ARG(7), ARG(7));
	{
		GEN_CLOSURE(array, ARG(8), 4, Z116_lambda, 0);
		COPY(ARG(2), &array[3]);
		LOAD_CLOSURE(array, ARG(8));
	}
	COPY(ARG(8), ARG(8));
	LOAD_GLOBFUN(&CZ115_lambda, ARG(9));
	LOAD_GLOBFUN(&Cnil_fun, ARG(10));
	LOAD_GLOBFUN(&Cundef_stream_op, ARG(11));
	LOAD_GLOBFUN(&Cundef_stream_op, ARG(12));
	LOAD_SYMBOL(SYMBOL(Slisp, 64), ARG(13));	/* STREAM */
	COPY(ARG(3), ARG(14));
	COPY(ARG(4), ARG(15));
	COPY(ARG(5), ARG(16));
	COPY(ARG(6), ARG(17));
	COPY(ARG(7), ARG(18));
	COPY(ARG(8), ARG(19));
	LOAD_GLOBFUN(&Cnil_fun, ARG(20));
	LOAD_GLOBFUN(&Cundef_stream_op, ARG(21));
	LOAD_GLOBFUN(&Cundef_stream_op, ARG(22));
	COPY(ARG(9), ARG(23));
	rt_make_struct(ARG(13), 11);
	COPY(ARG(13), ARG(0));
}

static void Z115_lambda(CL_FORM *base)
{
	LOAD_SMSTR((CL_FORM *)&Kmake_terminal_io[2], ARG(0));	/* can't close *terminal-io* */
}

static void Z116_lambda(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(ARG(0)) + 3), ARG(0));
}

static void Z117_lambda(CL_FORM *base)
{
	if(CL_CHARP(ARG(1)) && GET_CHAR(ARG(1)) == '\n')
	{
		LOAD_SMALLFIXNUM(0, INDIRECT(GET_FORM(ARG(0)) + 4));
	}
	else
	{
		COPY(INDIRECT(GET_FORM(ARG(0)) + 4), ARG(2));
		F1plus(ARG(2));
		COPY(ARG(2), INDIRECT(GET_FORM(ARG(0)) + 4));
	}
	COPY(ARG(1), ARG(2));
	COPY(INDIRECT(GET_FORM(ARG(0)) + 3), ARG(3));
	c_fputc(ARG(2));
	COPY(ARG(2), ARG(0));
}

static void Z118_lambda(CL_FORM *base)
{
	COPY(ARG(1), ARG(2));
	COPY(INDIRECT(GET_FORM(ARG(0)) + 3), ARG(3));
	c_ungetc(ARG(2));
	COPY(ARG(2), ARG(0));
}

static void Z119_lambda(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(ARG(0)) + 3), ARG(1));
	c_fgetc(ARG(1));
	COPY(ARG(1), ARG(0));
}
