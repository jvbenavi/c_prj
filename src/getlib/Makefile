
INCDIR = ../../inc
LIBDIR = ../../lib

CFLAGS = -I$(INCDIR) -Wall 
LDFLAGS = 

# libraries
LIBS = -L$(LIBDIR) -lgetlib

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

getlibfiles = \
	Cinteger.o \
	Gcommand.o \
	Gsignal.o \
	Preproc.o \
	get_Util.o \
	Clogical.o \
	Gerr.o \
	Icommand.o \
	Putpromp.o \
	globals.o \
	Creal.o	\
	Getnbl.o \
	Index.o \
	Real.o \
	linteger.o \
	Filename.o	\
	Ginput.o	\
	Integer.o	\
	Scommand.o \
	xcommand.o \
	Filename_ext.o \
	Gitem.o \
	Logical.o \
	Strtrm.o

all: test $(LIBDIR)/libgetlib.a

test: test.o $(LIBDIR)/libgetlib.a
	$(CC) $< $(LIBS) $(LDFLAGS) -o $@

$(LIBDIR)/libgetlib.a: $(getlibfiles)
	ar crv $@ $(getlibfiles)
	ranlib $@

%.o : %.c $(INCDIR)/getlib.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(DEL) *.o
	$(DEL) $(LIBDIR)/libgetlib.a
	$(DEL) test
