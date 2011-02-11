########################################################################
# prjone/utl/utils/Makefile
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
LIBUTIL = $(LIBDIR)/util.a
INCUTIL = $(INCDIR)/util.h

#
# TARGETS
#

all: $(LIBUTIL)

clean: 
	$(RM) $(LIBUTIL) $(INCUTIL)
	$(RM) *.o test

install:

uninstall:

############################################################

$(LIBUTIL): $(INCUTIL) \
			$(LIBUTIL)(util_file.o) \
		    $(LIBUTIL)(util_net.o) \
			$(LIBUTIL)(util_math.o) 
	$(RANLIB) $(RLIBARGS) $@
	@echo "$@ build is complete"

$(INCUTIL): util.h
	cp util.h $(INCUTIL)

LIBTEST = $(LIBDIR)/cutest.a
INCTEST = $(INCDIR)/CuTest.h
LIBS = $(LIBUTIL) $(LIBTEST)
INCS = $(INCUTIL) $(INCTEST)

test: AllTests.o util_test.o $(LIBS) $(INCS)
	$(RM) $@
	$(CC) $(CFLAGS) $(LIBS) $< util_test.o -o $@
	@echo "$@ build is complete"

