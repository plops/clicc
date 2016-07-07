#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kprint_float)
{
	MAKE_STRING(3, "0.0"),	/* 0 */
};

void print_float(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	Fminusp(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_CHAR('-', STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		Fwrite_char(STACK(base, 2), 2);
		mv_count = 1;
		COPY(STACK(base, 0), STACK(base, 2));
		Fminus(STACK(base, 2), 1);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	COPY(STACK(base, 0), STACK(base, 2));
	Fzerop(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&Kprint_float[0], STACK(base, 0));	/* 0.0 */
		Fwrite_string(STACK(base, 0), 2);
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		LOAD_FIXNUM(10, STACK(base, 3));
		log_internal(STACK(base, 2));
		LOAD_FIXNUM(1, STACK(base, 3));
		floor_internal(STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		{
			static double local_float = 10.0;
			LOAD_FLOAT(&local_float, STACK(base, 4));
		}
		LOAD_FIXNUM(-1, STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Fminus(STACK(base, 5), 2);
		Fexpt(STACK(base, 4));
		Fmult(STACK(base, 3), 2);
		LOAD_NIL(STACK(base, 4));
		COPY(SYMVAL(Slisp, 2), STACK(base, 5));	/* SHORT-FLOAT-EPSILON */
		LOAD_NIL(STACK(base, 6));
		COPY(STACK(base, 3), STACK(base, 7));
		LOAD_FIXNUM(1, STACK(base, 8));
		COPY(STACK(base, 5), STACK(base, 9));
		Fminus(STACK(base, 8), 2);
		Fge(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			static double local_float = 0.10000000000000001;
			LOAD_FLOAT(&local_float, STACK(base, 3));
			COPY(STACK(base, 2), STACK(base, 7));
			F1plus(STACK(base, 7));
			COPY(STACK(base, 7), STACK(base, 2));
		}
		COPY(STACK(base, 2), STACK(base, 7));
		LOAD_FIXNUM(7, STACK(base, 8));
		Fgt(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 8));
			LOAD_FIXNUM(-3, STACK(base, 9));
			Flt(STACK(base, 8), 2);
			bool_result = CL_TRUEP(STACK(base, 8));
		}
		if(bool_result)
		{
			LOAD_FIXNUM(0, STACK(base, 4));
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 4));
			LOAD_FIXNUM(0, STACK(base, 2));
		}
		COPY(STACK(base, 4), STACK(base, 7));
		Fminusp(STACK(base, 7));
		if(CL_TRUEP(STACK(base, 7)))
		{
			LOAD_CHAR('0', STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			Fwrite_char(STACK(base, 7), 2);
			mv_count = 1;
			LOAD_CHAR('.', STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			Fwrite_char(STACK(base, 7), 2);
			mv_count = 1;
			COPY(STACK(base, 4), STACK(base, 7));
			Fminus(STACK(base, 7), 1);
			F1minus(STACK(base, 7));
			LOAD_FIXNUM(0, STACK(base, 8));
			M107_1:;
			COPY(STACK(base, 8), STACK(base, 9));
			COPY(STACK(base, 7), STACK(base, 10));
			Fge(STACK(base, 9), 2);
			if(CL_TRUEP(STACK(base, 9)))
			{
				goto RETURN119;
			}
			LOAD_CHAR('0', STACK(base, 9));
			COPY(STACK(base, 1), STACK(base, 10));
			Fwrite_char(STACK(base, 9), 2);
			mv_count = 1;
			F1plus(STACK(base, 8));
			goto M107_1;
			RETURN119:;
			LOAD_FIXNUM(-1, STACK(base, 4));
		}
		M108_1:;
		COPY(STACK(base, 5), STACK(base, 7));
		LOAD_FIXNUM(10, STACK(base, 8));
		Fmult(STACK(base, 7), 2);
		COPY(STACK(base, 7), STACK(base, 5));
		LOAD_FIXNUM(10, STACK(base, 7));
		COPY(STACK(base, 3), STACK(base, 8));
		Fmult(STACK(base, 7), 2);
		LOAD_FIXNUM(1, STACK(base, 8));
		truncate_internal(STACK(base, 7));
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
			COPY(STACK(base, 7), STACK(base, 6));
			COPY(STACK(base, 8), STACK(base, 3));
		}
		COPY(STACK(base, 3), STACK(base, 7));
		LOAD_FIXNUM(1, STACK(base, 8));
		COPY(STACK(base, 5), STACK(base, 9));
		Fminus(STACK(base, 8), 2);
		Fge(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 3), STACK(base, 8));
			COPY(STACK(base, 5), STACK(base, 9));
			Fle(STACK(base, 8), 2);
			bool_result = CL_TRUEP(STACK(base, 8));
		}
		if(bool_result)
		{
			goto RETURN120;
		}
		COPY(STACK(base, 6), STACK(base, 7));
		Fdigit_char(STACK(base, 7), 1);
		COPY(STACK(base, 1), STACK(base, 8));
		Fwrite_char(STACK(base, 7), 2);
		mv_count = 1;
		COPY(STACK(base, 4), STACK(base, 7));
		Fzerop(STACK(base, 7));
		if(CL_TRUEP(STACK(base, 7)))
		{
			LOAD_CHAR('.', STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			Fwrite_char(STACK(base, 7), 2);
			mv_count = 1;
		}
		COPY(STACK(base, 4), STACK(base, 7));
		F1minus(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 4));
		goto M108_1;
		RETURN120:;
		COPY(STACK(base, 3), STACK(base, 7));
		{
			static double local_float = 0.5;
			LOAD_FLOAT(&local_float, STACK(base, 8));
		}
		Fge(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			F1plus(STACK(base, 6));
		}
		COPY(STACK(base, 6), STACK(base, 7));
		Fdigit_char(STACK(base, 7), 1);
		COPY(STACK(base, 1), STACK(base, 8));
		Fwrite_char(STACK(base, 7), 2);
		mv_count = 1;
		COPY(STACK(base, 4), STACK(base, 7));
		LOAD_FIXNUM(0, STACK(base, 8));
		Fge(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			LOAD_FIXNUM(0, STACK(base, 7));
			M109_1:;
			COPY(STACK(base, 7), STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 9));
			Fge(STACK(base, 8), 2);
			if(CL_TRUEP(STACK(base, 8)))
			{
				goto RETURN121;
			}
			LOAD_CHAR('0', STACK(base, 8));
			COPY(STACK(base, 1), STACK(base, 9));
			Fwrite_char(STACK(base, 8), 2);
			mv_count = 1;
			F1plus(STACK(base, 7));
			goto M109_1;
			RETURN121:;
			LOAD_CHAR('.', STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			Fwrite_char(STACK(base, 7), 2);
			mv_count = 1;
			LOAD_CHAR('0', STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			Fwrite_char(STACK(base, 7), 2);
			mv_count = 1;
		}
		COPY(STACK(base, 2), STACK(base, 7));
		Fzerop(STACK(base, 7));
		if(CL_TRUEP(STACK(base, 7)))
		{
		}
		else
		{
			LOAD_CHAR('E', STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			Fwrite_char(STACK(base, 7), 2);
			mv_count = 1;
			COPY(STACK(base, 2), STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			print_integer(STACK(base, 7));
			mv_count = 1;
		}
		LOAD_NIL(STACK(base, 0));
	}
}
