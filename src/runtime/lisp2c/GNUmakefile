#------------------------------------------------------------------------------
# CLiCC: The Common Lisp to C Compiler
# Copyright (C) 1994 Wolfgang Goerigk, Ulrich Hoffmann, Heinz Knutzen 
# Christian-Albrechts-Universitaet zu Kiel, Germany
#------------------------------------------------------------------------------
# Function : Generates Lisp runtime-modules for CLICC.
#            Uses optional parameter OPTIMIZE = <-{O | g}>.
#            Uses environment variable CLICCROOT.
#
#  $Revision: 1.14 $
#  $Id: GNUmakefile,v 1.14 1994/11/22 14:57:56 hk Exp $
#------------------------------------------------------------------------------

CC	 = gcc
LIB	 = $(CLICCROOT)/lib
OPTIMIZE = -O # alternative to -O is -g
CFLAGS	 = -I$(LIB) $(OPTIMIZE)
ARFLAGS	 = r
RANLIB   = ranlib
CAT      = cat
CP       = cp
MV       = mv
RM       = rm -f

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
	$(CAT) lisp.h >> lisp.hh
	$(MV) lisp.hh lisp.h
	$(CP) lisp.h $(LISPH)

$(INLINEH): inline.h
	$(CP) inline.h $(INLINEH)

$(SYNTAX): lisp.syntax
	$(CP) lisp.syntax $(SYNTAX)

$(DEF): lisp.def
	$(CP) lisp.def $(DEF)

$(LIBRARY): $(SOURCES)
	$(COMPILE.c) $?
	$(RM) $(LIBRARY)
	$(AR) $(ARFLAGS) $@ *.o
	$(RM) *.o
	$(RANLIB) $@
