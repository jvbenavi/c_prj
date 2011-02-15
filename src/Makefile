########################################################################
# prj/src/Makefile
# $Id$
########################################################################

SHELL=/bin/sh
MAKE_DIRS      = cutest util exe
INSTALL_DIRS   = $(MAKE_DIRS)
UNINSTALL_DIRS = $(MAKE_DIRS)

all: 
	@-for indir in $(MAKE_DIRS) ; do \
	  if [ -d $$indir ] ; then \
	    ( echo "\nWorking on $${indir}" ; \
	      cd $$indir ; \
	      make ) ; \
		if [ $$? -gt 0 ] ; then exit $$? ; fi ; \
	  fi ; \
	done

clean: 
	@-for indir in $(MAKE_DIRS) ; do \
	  if [ -d $$indir ] ; then \
	    ( echo "\nWorking on $${indir}" ; \
	      cd $$indir ; \
	      make clean ) ; \
		if [ $$? -gt 0 ] ; then exit $$? ; fi ; \
	  fi ; \
	done

install: 
	@-for indir in $(INSTALL_DIRS) ; do \
	  if [ -d $$indir ] ; then \
	    ( echo "\nWorking on $$indir" ; \
	      cd $$indir ; \
	      make install ) ; \
	  fi ; \
	done

uninstall: 
	@-for indir in $(UNINSTALL_DIRS) ; do \
	 ( echo "\nWorking on $$indir" ; \
	   cd $$indir ; \
	   make uninstall ) ; \
	done

########################################################################

