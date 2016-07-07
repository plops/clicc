#------------------------------------------------------------------------------
# Project  : APPLY - A Practicable And Portable Lisp Implementation
#            ------------------------------------------------------
# Function : Generates C runtime-modules for CLICC.
#            Uses optional parameter OPTIMIZE = <-{O | g}>.
#            Uses Environment variable CLICCROOT.
#
# $Revision: 1.6 $
# $Source: /home3/apply/public/clicc-0.6.2a/src/runtime/c/RCS/GNUmakefile,v $
# $Author: ft $
# $Date: 1993/07/15 15:14:34 $
#------------------------------------------------------------------------------

CC	= acc
OPTIMIZE = -O # alternative to -O is -g
CFLAGS	= -I$(CLICCROOT)/lib $(OPTIMIZE)
ARFLAGS	= r

ifeq ($(findstring -g,$(OPTIMIZE)), -g)
override CFLAGS := $(CFLAGS) -DDEBUG
endif

MAIN.C	= main.c
SOURCES 	= $(filter-out $(MAIN.C),$(wildcard *.c))

ifeq ($(findstring -g,$(OPTIMIZE)), -g)
 LIBRARY	= $(CLICCROOT)/lib/rtc-g.a
 MAIN.O 	= $(CLICCROOT)/lib/main-g.o
 INITIALIZE.O = $(CLICCROOT)/lib/initialize-g.o
else
 ifeq ($(findstring -pg,$(OPTIMIZE)), -pg)
   LIBRARY	= $(CLICCROOT)/lib/rtc-p.a
   MAIN.O 	= $(CLICCROOT)/lib/main-p.o
   INITIALIZE.O = $(CLICCROOT)/lib/initialize-p.o
 else
  LIBRARY	= $(CLICCROOT)/lib/rtc.a
  MAIN.O 	= $(CLICCROOT)/lib/main.o
  INITIALIZE.O = $(CLICCROOT)/lib/initialize.o
 endif
endif

.PRECIOUS: $(LIBRARY)

all: $(LIBRARY) $(MAIN.O) $(INITIALIZE.O)

$(LIBRARY): $(SOURCES)
	$(COMPILE.c) $?
	$(AR) $(ARFLAGS) $@ $(?:.c=.o)
	rm *.o
	ranlib $@

$(MAIN.O): $(MAIN.C)
	$(COMPILE.c) -o $@ $<

$(INITIALIZE.O): $(MAIN.C)
	$(COMPILE.c) -DNOMAIN -o $@ $<
