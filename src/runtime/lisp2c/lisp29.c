#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z36_g143(CL_FORM *base);
static void Z37_g142(CL_FORM *base);
static void Z38_g141(CL_FORM *base);
static void Z39_g140(CL_FORM *base);
static void Z40_g139(CL_FORM *base);
static void Z41_g137(CL_FORM *base);

void make_file_output_stream(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 2));
	LOAD_NIL(STACK(base, 2));
	GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 102), STACK(base, 3));	/* FILE-OUTPUT */
	{
		GEN_CLOSURE(array, STACK(base, 4), 5, Z41_g137, 1);
		COPY(STACK(base, 0), &array[3]);
		COPY(STACK(base, 2), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 4));
	}
	COPY(STACK(base, 4), STACK(base, 4));
	{
		GEN_CLOSURE(array, STACK(base, 5), 4, Z40_g139, 0);
		COPY(STACK(base, 2), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 5));
	}
	COPY(STACK(base, 5), STACK(base, 5));
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z39_g140, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	{
		GEN_CLOSURE(array, STACK(base, 7), 5, Z38_g141, 1);
		COPY(STACK(base, 0), &array[3]);
		COPY(STACK(base, 2), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 7));
	}
	COPY(STACK(base, 7), STACK(base, 7));
	{
		GEN_CLOSURE(array, STACK(base, 8), 4, Z37_g142, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 8));
	}
	COPY(STACK(base, 8), STACK(base, 8));
	{
		GEN_CLOSURE(array, STACK(base, 9), 4, Z36_g143, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 9));
	}
	COPY(STACK(base, 9), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 10));	/* STREAM */
	COPY(STACK(base, 3), STACK(base, 11));
	COPY(STACK(base, 1), STACK(base, 12));
	LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 13));
	LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 14));
	COPY(STACK(base, 4), STACK(base, 15));
	COPY(STACK(base, 5), STACK(base, 16));
	COPY(STACK(base, 6), STACK(base, 17));
	COPY(STACK(base, 7), STACK(base, 18));
	COPY(STACK(base, 8), STACK(base, 19));
	COPY(STACK(base, 9), STACK(base, 20));
	make_struct(STACK(base, 10), 11);
	COPY(STACK(base, 10), STACK(base, 0));
}

static void Z36_g143(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	c_fclose(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 2));
	close1(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z37_g142(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_file_length(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z38_g141(CL_FORM *base)
{
	LOAD_NIL(INDIRECT(GET_FORM(STACK(base, 0)) + 4));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	c_fseek(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z39_g140(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_ftell(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z40_g139(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
}

static void Z41_g137(CL_FORM *base)
{
	if(CL_CHARP(STACK(base, 1)) && GET_CHAR(STACK(base, 1)) == '\n')
	{
		LOAD_FIXNUM(0, INDIRECT(GET_FORM(STACK(base, 0)) + 4));
	}
	else
	{
		if(CL_TRUEP(INDIRECT(GET_FORM(STACK(base, 0)) + 4)))
		{
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
			F1plus(STACK(base, 2));
			COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 4));
		}
	}
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	c_fputc(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}
