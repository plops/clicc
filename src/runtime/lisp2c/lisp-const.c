#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KClisp)
{
	MAKE_STRING(16, "~a is not a cons"),	/* 0 */
	MAKE_STRING(16, "~a is not a cons"),	/* 2 */
	MAKE_STRING(22, "extra argument for #~S"),	/* 4 */
	MAKE_STRING(16, "~a is not a cons"),	/* 6 */
	MAKE_STRING(32, "type error: ~S is not of type ~S"),	/* 8 */
	MAKE_STRING(39, "~S cannot be represented relative to ~S"),	/* 10 */
	MAKE_STRING(16, "~a is not a cons"),	/* 12 */
	MAKE_STRING(58, "~S: The slot ~s is missing from the object ~s of class ~s."),	/* 14 */
	MAKE_STRING(52, "The slot ~s is unbound in the object ~s of class ~s."),	/* 16 */
	MAKE_STRING(122, "~S ist not a valid argument for CLASS-OF, ~\n              these have been restricted to instances of user-defined-classes."),	/* 18 */
	MAKE_STRING(19, "~&Error in ~A: ~?~%"),	/* 20 */
};
