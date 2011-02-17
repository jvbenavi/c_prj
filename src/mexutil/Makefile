########################################################################
# prjone/src/mex/Makefile
# $Id$
########################################################################

# Directory paths 
TOPDIR = ../..
BINDIR = $(TOPDIR)/bin
INCDIR = $(TOPDIR)/inc
LIBDIR = $(TOPDIR)/lib

CXTRA  = -g -Wall -I$(INCDIR) -I/Applications/MATLAB_R2011a.app/extern/include

# Definitions and implicit rules for makefiles          
include $(TOPDIR)/cfg/makefile_rules

# libraries 
LIBMEXUTIL = $(LIBDIR)/libmexutil.a
INCMEXUTIL = $(INCDIR)/mexutil.h

#
# TARGETS
#

all: $(LIBMEXUTIL)

clean: 
	$(RM) $(LIBMEXUTIL) 
#	$(RM) *.o $(BINDIR)/testUtil

install:

uninstall:

ckeck:
#check: $(BINDIR)/testUtil
#	@$(BINDIR)/testUtil

############################################################

$(LIBMEXUTIL): $(INCMEXUTIL) \
			$(LIBMEXUTIL)(mexutil.o) 
	$(RANLIB) $(RLIBARGS) $@
	@echo "$@ build is complete"

#LIBTEST = $(LIBDIR)/cutest.a
#INCTEST = $(INCDIR)/CuTest.h
#LIBS = $(LIBUTIL) $(LIBTEST)
#INCS = $(INCUTIL) $(INCTEST)

#$(BINDIR)/testUtil: AllTests.o util_test.o $(LIBS) $(INCS)
#	$(RM) $@
#	$(CC) $(CFLAGS) $(LIBS) $< util_test.o -o $@
#	@echo "$@ build is complete"

