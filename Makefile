MYCC	= clang
MYCC	= g++

CXX_OPT	= -g #-O9

CXX	= $(MYCC) -std=c++11 -Wall  -Wconversion 	\
		-D_FILE_OFFSET_BITS=64	\
		-MMD -MP		\
		$(CXX_OPT)		\
		-c

LINK	= $(MYCC) -o
LIBS	= -lstdc++

#-ljemalloc
#-fpack-struct

SRC	= $(wildcard *.cc)


TARGETS	=	\
		db_file db_lsm db_merge	\
		\
		test_stringref	\
		test_glob	\
		test_stl	\
		\
		test_pair	\
		test_list	\
		test_file


all: $(TARGETS)


clean:
	rm -f *.o *.d *.gch		\
			$(TARGETS)

test_stringref: test_stringref.o
	$(LINK) $@ $^		$(LIBS)

test_pair: test_pair.o	pair.o pairblob.o nmea0183checksumcalculator.o mytime.o
	$(LINK) $@ $^		$(LIBS)

test_list: test_list.o	pair.o nmea0183checksumcalculator.o mytime.o vectorlist.o linklist.o skiplist.o
	$(LINK) $@ $^		$(LIBS)

test_file: test_file.o	pair.o nmea0183checksumcalculator.o mytime.o vectorlist.o linklist.o skiplist.o disktable.o mmapfile.o diskfileheader.o
	$(LINK) $@ $^		$(LIBS)

test_stl: test_stl.o	pair.o nmea0183checksumcalculator.o mytime.o
	$(LINK) $@ $^		$(LIBS)

test_glob: test_glob.o myglob.o
	$(LINK) $@ $^		$(LIBS)

db_file: db_file.o	pair.o nmea0183checksumcalculator.o mytime.o disktable.o mmapfile.o diskfileheader.o
	$(LINK) $@ $^		$(LIBS)

db_lsm: db_lsm.o	pair.o nmea0183checksumcalculator.o mytime.o disktable.o mmapfile.o diskfileheader.o myglob.o
	$(LINK) $@ $^		$(LIBS)

db_merge: db_merge.o	pair.o nmea0183checksumcalculator.o mytime.o disktable.o mmapfile.o diskfileheader.o
	$(LINK) $@ $^		$(LIBS)


%.o: %.cc
	$(CXX) $<

-include $(SRC:%.cc=%.d)

