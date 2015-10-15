MYCC	= clang
MYCC	= g++

CC_OPT	= -g #-O9

CC	= $(MYCC) -std=c++14 -Wall	\
		-D_FILE_OFFSET_BITS=64	\
		-MMD -MP		\
		$(CC_OPT)		\
		-c

LINK	= $(MYCC) -o
LIBS	= -lstdc++

#-ljemalloc
#-fpack-struct

SRC	= $(wildcard *.cc)



TARGETS	=	\
		test_stringref	\
		test_pair	\
		test_list	\
		test_file	\
		test_stl	\
		test_glob	\
		test_dir


all: $(TARGETS)


clean:
	rm -f *.o *.d *.gch		\
			$(TARGETS)

test_stringref: test_stringref.o
	$(LINK) $@ $^		$(LIBS)

test_pair: test_pair.o pair.o nmea0183checksumcalculator.o mytime.o
	$(LINK) $@ $^		$(LIBS)

test_list: test_list.o pair.o nmea0183checksumcalculator.o mytime.o vectorlist.o linklist.o skiplist.o
	$(LINK) $@ $^		$(LIBS)

test_file: test_file.o pair.o nmea0183checksumcalculator.o mytime.o vectorlist.o linklist.o skiplist.o disktable.o
	$(LINK) $@ $^		$(LIBS)

test_dir: test_dir.o pair.o nmea0183checksumcalculator.o mytime.o myglob.o disktable.o multitabledirectory.o
	$(LINK) $@ $^		$(LIBS)

test_stl: test_stl.o pair.o nmea0183checksumcalculator.o mytime.o
	$(LINK) $@ $^		$(LIBS)

test_glob: test_glob.o myglob.o
	$(LINK) $@ $^		$(LIBS)

%.o: %.cc
	$(CC) $<

-include $(SRC:%.cc=%.d)

