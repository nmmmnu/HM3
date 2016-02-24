MYCC	= clang
MYCC	= g++

CXX_OPT	= -g
#CXX_OPT	= -O9
#CXX_OPT	=

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
		test_stringref		\
		test_glob		\
		test_idgenerator	\
		test_stl		\
		\
		test_pair		\
		test_list		\
		test_file

PAIR_OBJ = pair.o pairblob.o nmea0183checksumcalculator.o mytime.o
DISKTABLE_OBJ = disktable.o mmapfile.o diskfileheader.o

all: $(TARGETS)


clean:
	rm -f *.o *.d *.gch		\
			$(TARGETS)

test_stringref: test_stringref.o
	$(LINK) $@ $^		$(LIBS)

test_glob: test_glob.o myglob.o
	$(LINK) $@ $^		$(LIBS)

test_idgenerator: test_idgenerator.o idgenerator.o mytime.o
	$(LINK) $@ $^		$(LIBS)

test_stl: test_stl.o	$(PAIR_OBJ)
	$(LINK) $@ $^		$(LIBS)

test_pair: test_pair.o	$(PAIR_OBJ)
	$(LINK) $@ $^		$(LIBS)

test_list: test_list.o	$(PAIR_OBJ) stlvectorlist.o vectorlist.o linklist.o skiplist.o
	$(LINK) $@ $^		$(LIBS)

test_file: test_file.o	$(PAIR_OBJ) stlvectorlist.o vectorlist.o linklist.o skiplist.o $(DISKTABLE_OBJ)
	$(LINK) $@ $^		$(LIBS)


db_file: db_file.o	$(PAIR_OBJ) $(DISKTABLE_OBJ)
	$(LINK) $@ $^		$(LIBS)

db_lsm: db_lsm.o	$(PAIR_OBJ) $(DISKTABLE_OBJ) directorylsmcontainer.o myglob.o
	$(LINK) $@ $^		$(LIBS)

db_merge: db_merge.o	$(PAIR_OBJ) $(DISKTABLE_OBJ) arglsmcontainer.o
	$(LINK) $@ $^		$(LIBS)

test_duallist: test_duallist.o	$(PAIR_OBJ) $(DISKTABLE_OBJ) skiplist.o
	$(LINK) $@ $^		$(LIBS)


%.o: %.cc
	$(CXX) $<

-include $(SRC:%.cc=%.d)

