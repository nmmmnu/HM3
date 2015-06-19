MYCC	= clang
MYCC	= g++

CC	= $(MYCC) -Wall			\
		-D_FILE_OFFSET_BITS=64	\
		-c
LINK	= $(MYCC) -o
LIBS	=
#-ljemalloc
#-fpack-struct

TARGETS =	pair.o nmea0183checksumcalculator.o


all: $(TARGETS)


clean:
	rm -f *.o $(TARGETS)



pair.o: pair.cc pair.h ipair.h
	$(CC) pair.cc

nmea0183checksumcalculator.o: nmea0183checksumcalculator.cc nmea0183checksumcalculator.h ichecksumcalculator.h
	$(CC) nmea0183checksumcalculator.cc
