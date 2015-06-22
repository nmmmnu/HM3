MYCC	= clang
MYCC	= g++

CC	= $(MYCC) -std=c++11 -Wall	\
		-D_FILE_OFFSET_BITS=64	\
		-c
LINK	= $(MYCC) -o
LIBS	= -lstdc++
#-ljemalloc
#-fpack-struct

TARGETS =	\
		test_pair	\
		test_list


all: $(TARGETS)


clean:
	rm -f *.o $(TARGETS) a.out


mytime.o: mytime.cc mytime.h
	$(CC) mytime.cc

pair.o: pair.cc pair.h mytime.h
	$(CC) pair.cc

nmea0183checksumcalculator.o: nmea0183checksumcalculator.cc nmea0183checksumcalculator.h ichecksumcalculator.h
	$(CC) nmea0183checksumcalculator.cc

iiterator.o: iiterator.cc iiterator.h pair.h
	$(CC) iiterator.cc

vectorlist.o: vectorlist.cc vectorlist.h ilist.h iiterator.h pair.h
	$(CC) vectorlist.cc

linklist.o: linklist.cc linklist.h ilist.h iiterator.h pair.h
	$(CC) linklist.cc

skiplist.o: skiplist.cc skiplist.h ilist.h iiterator.h pair.h
	$(CC) skiplist.cc

test_pair.o: test_pair.cc pair.h nmea0183checksumcalculator.h ichecksumcalculator.h
	$(CC) test_pair.cc

test_pair: 			test_pair.o	pair.o nmea0183checksumcalculator.o mytime.o
	$(LINK) test_pair	test_pair.o	pair.o nmea0183checksumcalculator.o mytime.o	$(LIBS)

test_list.o: test_list.cc nullsafelist.h skiplist.h linklist.h vectorlist.h pair.h
	$(CC) test_list.cc

test_list:			test_list.o	skiplist.o linklist.o vectorlist.o iiterator.o	mytime.o pair.o nmea0183checksumcalculator.o
	$(LINK) test_list	test_list.o	skiplist.o linklist.o vectorlist.o iiterator.o	mytime.o pair.o nmea0183checksumcalculator.o	$(LIBS)

