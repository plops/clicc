#------------------------------------------------------------------------------
#  Projekt : CLICC - a Common Lisp to C Compiler
#            -----------------------------------
# Function : Generates Lisp runtime-modules for CLICC.
#            Uses optional parameter OPTIMIZE = <-{O | g}>.
#            Uses environment variable CLICCROOT.
#
#  $Source: /home3/apply/public/clicc-0.6.2a/src/runtime/lisp2c/RCS/GNUmakefile,v $
#  $Author: ft $
#  $Revision: 1.8 $
#  $Date: 1993/07/15 15:14:13 $
#------------------------------------------------------------------------------

CC	= acc
LIB	= $(CLICCROOT)/lib
OPTIMIZE = -g # alternative to -O is -g
CFLAGS	= -I$(LIB) $(OPTIMIZE)
ARFLAGS	= r

SOURCES 	= $(wildcard *.c)
SYNTAX		= $(LIB)/lisp.syntax
DEF		= $(LIB)/lisp.def
LISPH		= $(LIB)/lisp.h
INLINEH		= $(LIB)/inline.h

ifeq ($(findstring -g,$(OPTIMIZE)), -g)
 LIBRARY	= $(LIB)/rtl-g.a
else
 ifeq ($(findstring -pg,$(OPTIMIZE)), -pg)
 LIBRARY	= $(LIB)/rtl-p.a
 else
  LIBRARY	= $(LIB)/rtl.a
 endif
endif

.PRECIOUS: $(LIBRARY)

all:  $(LISPH) $(INLINEH) $(LIBRARY) $(SYNTAX) $(DEF)

$(LISPH): lisp.h	
	echo '#include "inline.h"' > lisp.hh
	cat lisp.h >> lisp.hh
	mv lisp.hh lisp.h
	cp lisp.h $(LISPH)

$(INLINEH): inline.h
	cp inline.h $(INLINEH)

$(SYNTAX): lisp.syntax
	cp lisp.syntax $(SYNTAX)

$(DEF): lisp.def
	cp lisp.def $(DEF)

$(LIBRARY): $(SOURCES)
	-$(COMPILE.c) $?
	$(AR) $(ARFLAGS) $@ *.o
	rm *.o
	ranlib $@