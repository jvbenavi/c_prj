########################################################################
# prjone/utl/cutest/Makefile
########################################################################

# Directory paths 
TOPDIR = ../..
BINDIR = $(TOPDIR)/run/bin
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
	$(RM) $(LIBTEST) $(INCTEST)
	$(RM) *.o test

install:

uninstall:

############################################################

$(LIBTEST): $(INCTEST) \
			$(LIBTEST)(CuTest.o) 
	$(RANLIB) $(RLIBARGS) $@
	@echo "$@ build is complete"

$(INCTEST): CuTest.h
	cp CuTest.h $(INCTEST)

test: AllTests.o CuTestTest.o $(LIBTEST) $(INCTEST)
	$(RM) $@
	$(CC) $(CFLAGS) $(LIBTEST) $< CuTestTest.o -o $@
	@echo "$@ build is complete"



