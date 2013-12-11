# -- Configurable
#CDEFINES=-O1 -DMACRO_SINCOS -DSGDP4_SNGL
CDEFINES=-O2 -DMACRO_SINCOS
#CDEFINES=-O2

# -- Linux
# CC=gcc
# CFLAGS=-Wall -DLINUX $(CDEFINES)
# XLIBS=

# -- NonLinux
CC=gcc
CFLAGS=-Wall $(CDEFINES)
RANLIB=ranlib
XLIBS=

RM=rm -f
AR=ar cr
RANLIB=ranlib
DEPFLAGS=-I/usr/include

SRCS=aries.c deep.c ferror.c satutl.c sgdp4.c test1.c

OBJS=${SRCS:.c=.o}

LIB=-lm $(XLIBS)

all: testsgp compvec

testsgp: $(OBJS)
	$(CC) $(CFLAGS) $(DEPFLAGS) -o $@ $(OBJS) $(LIB)

compvec: comp.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -o $@ comp.c $(LIB)

test:
	$(RM) ./test_?.txt
	cp ../data/twoline.txt twoline.txt
	cp ../data/ssd_b.txt ssd.txt
	./testsgp > ../result/test_b.txt
	./compvec ../data/test_b.txt ../data/test_example.txt 0 > ../result/diff.txt
	$(RM) ssd.txt twoline.txt

clean:
	$(RM) core $(OBJS)

cleanall:
	$(RM) core $(OBJS) testsgp compvec ../result/diff.txt ../result/test_b.txt *.OBJ *.BAK *.exe *.bak

.c.o:
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $*.c

depend:
	makedepend $(CFLAGS) $(DEPFLAGS) $(SRCS)
	@sed -f nosysdep.sed < Makefile > Makefile.tmp
	@mv Makefile.tmp Makefile
	@mv Makefile.bak .Makefile

# DO NOT DELETE THIS LINE -- make depend depends on it.
