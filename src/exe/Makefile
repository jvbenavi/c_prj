########################################################################
# prjone/src/exe/Makefile
# $Id$
########################################################################

# Directory paths 
TOPDIR = ../..
BINDIR = $(TOPDIR)/bin
INCDIR = $(TOPDIR)/inc
LIBDIR = $(TOPDIR)/lib

PPGDIR = $(TOPDIR)/displays/simlabs/ppgd
MODEL  = PPGD_NAVD_100604
RTWDIR = $(PPGDIR)/$(MODEL)_grt_rtw
MATDIR = /Applications/MATLAB_R2009b.app
SLKDIR = $(MATDIR)/simulink/include
MATEXT = $(MATDIR)/extern/include
#CXTRA  = -I$(PPGDIR) -I$(XFZDIR) -I$(XIODIR) -I$(RTWDIR) -I$(MATEXT) -I$(SLKDIR) 

CXTRA  = -g -Wall -I$(INCDIR) -fno-common # force proper use of extern

# Linker flags:
#LDXTRA = -lm -lstdc++

# Definitions and implicit rules for makefiles          
include $(TOPDIR)/cfg/makefile_rules

# libraries
LIBS = $(LIBDIR)/libutil.a
INCS = $(INCDIR)/util.h

#
# TARGETS
#

all: $(BINDIR)/prj

clean:
	$(RM) *.o $(BINDIR)/prj

install:

uninstall:

check: $(BINDIR)/prj
	@$(BINDIR)/prj 

############################################################

$(BINDIR)/prj: prj_main.o func1.o func2.o $(LIBS) $(INCS)
	$(RM) $@
	$(CC) $(CFLAGS) $(LIBS) $< func1.o func2.o -o $@ 
	@echo "$@ build is complete"

