# -- Configurable
#CDEFINES=-O1 -DMACRO_SINCOS -DSGDP4_SNGL
CDEFINES=-O2 -DMACRO_SINCOS
#CDEFINES=-O2

# -- Linux
# CC=gcc
# CFLAGS=-Wall -DLINUX $(CDEFINES)
# XLIBS=

# -- CygWin
CC=gcc
CFLAGS=-Wall $(CDEFINES)
RANLIB=ranlib
XLIBS=

RM=rm -f
AR=ar cr
RANLIB=ranlib
DEPFLAGS=-I/usr/include
# SRCDIR=./src/

# -- create space variable
comma:= ,
empty:=
space:= $(empty) $(empty)

SRCS=aries.c deep.c ferror.c satutl.c sgdp4.c test1.c

# OBJS=$(SRCDIR)$(subst  $(space),$(space)$(SRCDIR),${SRCS:.c=.o})
OBJS=${SRCS:.c=.o}

LIB=-lm $(XLIBS)

all: testsgp compvec

testsgp: $(OBJS)
	$(CC) $(CFLAGS) $(DEPFLAGS) -o $@ $(OBJS) $(LIB)

compvec: comp.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -o $@ comp.c $(LIB)

test:
	$(RM) ./test_?.txt
	cp ../data/twoline.txt ./twoline.txt
	cp ../data/ssd_b.txt ssd.txt
	./testsgp > ../data/test_b.txt
	./compvec ../data/test_b.txt test_example.txt 0 > diff.txt

    # ./testsgp -i catalog_3l_2006_06_26_pm.txt > /dev/null

clean:
	$(RM) core $(OBJS)

cleanall:
	$(RM) core $(OBJS) *.OBJ *.BAK *.exe *.bak

.c.o:
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $*.c

depend:
	makedepend $(CFLAGS) $(DEPFLAGS) $(SRCS)
	@sed -f nosysdep.sed < Makefile > Makefile.tmp
	@mv Makefile.tmp Makefile
	@mv Makefile.bak .Makefile

# DO NOT DELETE THIS LINE -- make depend depends on it.
