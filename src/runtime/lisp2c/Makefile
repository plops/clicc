#------------------------------------------------------------------------------
# CLiCC: The Common Lisp to C Compiler
# Copyright (C) 1994 Wolfgang Goerigk, Ulrich Hoffmann, Heinz Knutzen 
# Christian-Albrechts-Universitaet zu Kiel, Germany
#------------------------------------------------------------------------------
# Function : Generates Lisp runtime-modules for CLICC.
#            Uses optional parameter OPTIMIZE = <-{O | g}>.
#            Uses environment variable CLICCROOT.
#
#  $Revision: 1.3 $
#  $Id: Makefile,v 1.3 1994/11/22 14:57:56 hk Exp $
#------------------------------------------------------------------------------

CC	= acc
LIB	= $(CLICCROOT)/lib
OPTIMIZE = -O # alternative to -O is -g
CFLAGS	=  -I$(LIB) $(OPTIMIZE)
ARFLAGS	= r


SOURCES:sh = echo *.c
#OBJECTS	= $(SOURCES:.c=.o)

LIBRARY	= $(LIB)/rtl.a

.KEEP_STATE:
.PRECIOUS: $(LIBRARY)

all: $(LIBRARY)

#$(LIBRARY): $(LIBRARY)($(OBJECTS))
#	ranlib $@

$(LIBRARY): $(SOURCES)
	echo '#include "inline.h"' > lisp.hh
	cat lisp.h >> lisp.hh
	mv lisp.hh lisp.h
	-$(COMPILE.c) $?
	$(AR) $(ARFLAGS) $@ $(?:.c=.o)
	?rm $(?:.c=.o)
	ranlib $@
	cp lisp.syntax $(LIB)
	cp inline.h lisp.h $(LIB)
