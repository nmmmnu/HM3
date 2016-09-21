MYCC		= clang

CF_OPTIM	= -O3

CF_ALL		= -std=c++11	\
			$(CF_DEPS)	\
			$(CF_INCL)	\
			$(CF_OPTIM)	\
			$(CF_WARN)	\
			$(CF_MISC)

CXX		= $(MYCC) $(CF_ALL)

LL_ALL		= -lstdc++ -lz

LINK		= $(MYCC) $(LD_ALL) -o $@ $^ $(LL_ALL)

all: test

clean:
	rm -f *.o test

test: test.o zlib.o zlibeasy.o
	$(LINK)

%.o: %.cc
	$(CXX) \
			-c $<		\
			-o $@

