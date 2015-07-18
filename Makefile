MYCC	= clang
MYCC	= g++

CC	= $(MYCC) -std=c++11 -Wall	\
		-D_FILE_OFFSET_BITS=64	\
		-MMD -MP		\
		-c

LINK	= $(MYCC) -o
LIBS	= -lstdc++

#-ljemalloc
#-fpack-struct

SRC	=	\
		pair.cc diskfile.cc			\
		iiterator.cc iarray.cc			\
		vectorlist.cc linklist.cc skiplist.cc	\
		disktable.cc dirtable.cc		\
		nmea0183checksumcalculator.cc		\
		mytime.cc myglob.cc



OBJECTS	=	$(SRC:%.cc=%.o)

TARGETS	=	\
		test_pair	\
		test_list	\
		test_file	\
		test_glob	\
		test_dir


all: $(TARGETS)


clean:
	rm -f *.o *.d $(TARGETS)

test_pair: test_pair.o pair.o nmea0183checksumcalculator.o mytime.o
	$(LINK) $@ $^		$(LIBS)

test_list: test_list.o pair.o mytime.o iiterator.o iarray.o vectorlist.o linklist.o  skiplist.o
	$(LINK) $@ $^		$(LIBS)

test_file: test_file.o pair.o mytime.o iiterator.o iarray.o vectorlist.o linklist.o  skiplist.o  disktable.o diskfile.o
	$(LINK) $@ $^		$(LIBS)

test_glob: test_glob.o myglob.o
	$(LINK) $@ $^		$(LIBS)

test_dir: test_dir.o pair.o mytime.o iiterator.o iarray.o myglob.o disktable.o dirtable.o
	$(LINK) $@ $^		$(LIBS)

%.o: %.cc
	$(CC) $<

-include $(SRC:%.cc=%.d)

