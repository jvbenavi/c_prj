########################################################################
# prjone/src/cutest/Makefile
# $Id$
########################################################################

# Directory paths 
TOPDIR = ../..
BINDIR = $(TOPDIR)/bin
INCDIR = $(TOPDIR)/inc
LIBDIR = $(TOPDIR)/lib

CXTRA  = -g -Wall -I$(INCDIR)

# Definitions and implicit rules for makefiles          
include $(TOPDIR)/cfg/makefile_rules

# libraries 
LIBTEST = $(LIBDIR)/cutest.a
INCTEST = $(INCDIR)/CuTest.h

#
# TARGETS
#

all: $(LIBTEST)

clean: 
	$(RM) $(LIBTEST) 
	$(RM) *.o $(BINDIR)/testCuTest

install:

uninstall:

check: $(BINDIR)/testCuTest
	@$(BINDIR)/testCuTest 

############################################################

$(LIBTEST): $(INCTEST) \
			$(LIBTEST)(CuTest.o) 
	$(RANLIB) $(RLIBARGS) $@
	@echo "$@ build is complete"

$(BINDIR)/testCuTest: AllTests.o CuTestTest.o $(LIBTEST) $(INCTEST)
	$(RM) $@
	$(CC) $(CFLAGS) $(LIBTEST) $< CuTestTest.o -o $@
	@echo "$@ build is complete"


