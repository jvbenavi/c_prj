########################################################################
# prj/Makefile
# $Id$
########################################################################

SHELL=/bin/sh
MAKE_DIRS      = src
INSTALL_DIRS   = cfg $(MAKE_DIRS)
UNINSTALL_DIRS = $(MAKE_DIRS) cfg

all: cfg/makefile_rules
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
	@make clean
	@-for indir in $(UNINSTALL_DIRS) ; do \
	 ( echo "\nWorking on $$indir" ; \
	   cd $$indir ; \
	   make uninstall ) ; \
	done
	@find . -name ".DS_Store" -delete

########################################################################

cfg/makefile_rules:
	@echo 'New Configuration (makefile_rules missing)'
	@echo 'Doing make install'
	make install

