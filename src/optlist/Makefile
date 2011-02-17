############################################################################
# Makefile for optlist command line library sample
#
#   $Id$
#   $Log: Makefile,v $
#   Revision 1.1.1.2  2007/09/04 04:45:42  michael
#   Added FreeOptList.
#
#   Revision 1.1.1.1  2007/08/07 05:01:48  michael
#   Initial Release
#
#
############################################################################
CC = gcc
LD = gcc

INCDIR = ../../inc
LIBDIR = ../../lib

CFLAGS = -I$(INCDIR) -O3 -Wall -pedantic -ansi -c
LDFLAGS = -O3 -o

# libraries
LIBS = -L$(LIBDIR) -loptlist

# Treat NT and non-NT windows the same
ifeq ($(OS),Windows_NT)
    OS = Windows
endif

ifeq ($(OS),Windows)
    EXE = .exe
    DEL = del
else    #assume Linux/Unix
    EXE =
    DEL = rm -rf
endif

all:		sample$(EXE) $(LIBDIR)/liboptlist.a

sample$(EXE):	sample.o $(LIBDIR)/liboptlist.a
	$(LD) $< $(LIBS) $(LDFLAGS) $@

sample.o:	sample.c $(INCDIR)/optlist.h
	$(CC) $(CFLAGS) $<

$(LIBDIR)/liboptlist.a:	optlist.o
	ar crv $@ optlist.o
	ranlib $@

optlist.o:	optlist.c $(INCDIR)/optlist.h
	$(CC) $(CFLAGS) $<

clean:
	$(DEL) *.o
	$(DEL) $(LIBDIR)/liboptlist.a
	$(DEL) sample$(EXE)
