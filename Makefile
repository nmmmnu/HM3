MYCC	= clang
MYCC	= g++

CC	= $(MYCC) -std=c++11 -Wall	\
		-D_FILE_OFFSET_BITS=64	\
		-MMD			\
		-c

LINK	= $(MYCC) -o
LIBS	= -lstdc++

#-ljemalloc
#-fpack-struct

SRC	=	\
		pair.cc					\
		iiterator.cc				\
		vectorlist.cc linklist.cc skiplist.cc	\
		nmea0183checksumcalculator.cc		\
		mytime.cc

OBJECTS	=	$(SRC:%.cc=%.o)

TARGETS	=	\
		test_pair	\
		test_list

all: $(TARGETS)


clean:
	rm -f *.o .deps/*.d $(TARGETS)

test_pair: test_pair.o		$(OBJECTS)
	$(LINK) $@	\
		$^	\
			$(LIBS)

test_list: test_list.o		$(OBJECTS)
	$(LINK) $@	\
		$^	\
			$(LIBS)

%.o: %.cc
	$(CC) $<

-include $(SRC:%.cc=%.d)

