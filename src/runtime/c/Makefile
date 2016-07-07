#------------------------------------------------------------------------------
# CLiCC: The Common Lisp to C Compiler
# Copyright (C) 1994 Wolfgang Goerigk, Ulrich Hoffmann, Heinz Knutzen 
# Christian-Albrechts-Universitaet zu Kiel, Germany
#------------------------------------------------------------------------------
# Function : Generates C runtime-modules for CLICC.
#            Uses optional parameter OPTIMIZE = <-{O | g}>.
#            Uses Environment variable CLICCROOT.
#
# $Revision: 1.7 $
# $Source: /home2/karlo/uho/clicc-pre-0.6.5/src/runtime/c/RCS/Makefile,v $
# $Author: hk $
# $Date: 1994/11/22 14:54:01 $
#------------------------------------------------------------------------------

CC	= acc
OPTIMIZE = -O # alternative to -O is -g
CFLAGS	= -I$(CLICCROOT)/lib $(OPTIMIZE)
ARFLAGS	= r

SOURCES:sh = echo *.c
#OBJECTS	= $(SOURCES:.c=.o)

LIBRARY	= $(CLICCROOT)/lib/rtc.a
MAIN.O 	= $(CLICCROOT)/lib/main.o
INITIALIZE.O = $(CLICCROOT)/lib/initialize.o

.KEEP_STATE:
.PRECIOUS: $(LIBRARY)

all: $(LIBRARY) $(MAIN.O) $(INITIALIZE.O)

#$(LIBRARY): $(LIBRARY)($(OBJECTS))
#	ranlib $@

$(LIBRARY): $(SOURCES)
	$(COMPILE.c) $?
	$(AR) $(ARFLAGS) $@ $(?:.c=.o)
	?rm $(?:.c=.o)
	?ranlib $@

$(MAIN.O): $(MAIN.C)
	$(COMPILE.c) -o $@ $<

$(INITIALIZE.O): $(MAIN.C)
	$(COMPILE.c) -DNOMAIN -o $@ $<

