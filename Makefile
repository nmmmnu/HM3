MYCC	= clang
MYCC	= g++

CC	= $(MYCC) -Wall			\
		-D_FILE_OFFSET_BITS=64	\
		-c
LINK	= $(MYCC) -o
LIBS	= -lstdc++
#-ljemalloc
#-fpack-struct

TARGETS =	test_pair


all: $(TARGETS)


clean:
	rm -f *.o $(TARGETS) a.out


mytime.o: mytime.cc mytime.h
	$(CC) mytime.cc

pair.o: pair.cc pair.h mytime.h
	$(CC) pair.cc

nmea0183checksumcalculator.o: nmea0183checksumcalculator.cc nmea0183checksumcalculator.h ichecksumcalculator.h
	$(CC) nmea0183checksumcalculator.cc

test_pair.o: test_pair.cc pair.h nmea0183checksumcalculator.h ichecksumcalculator.h
	$(CC) test_pair.cc

test_pair: 			test_pair.o	mytime.o pair.o nmea0183checksumcalculator.o
	$(LINK) test_pair	test_pair.o	mytime.o pair.o nmea0183checksumcalculator.o	$(LIBS)


