MYCC		= clang
MYCC		= g++

# ======================================================

EXTRA_CLEAN	:=
EXTRA_INCL	:=

# ======================================================

CF_DEPS		= -MMD -MP
CF_INCL		= -Iinclude $(EXTRA_INCL)
CF_OPTIM	=
CF_WARN		= -Wall -Wconversion -Wpedantic

CF_MISC		= -D_FILE_OFFSET_BITS=64

CF_ALL		= -std=c++11	\
			$(CF_DEPS)	\
			$(CF_INCL)	\
			$(CF_OPTIM)	\
			$(CF_WARN)	\
			$(CF_MISC)

CXX		= $(MYCC) $(CF_ALL)

# ======================================================

LD_ALL		=
LL_ALL		= -lstdc++

LINK		= $(MYCC) $(LD_ALL) -o $@ $^ $(LL_ALL)

SRC		= $(wildcard *.cc)

# ======================================================

A		= bin/
O		= obj/

# ======================================================
# ======================================================
# ======================================================

SUBDIRS = test common checksumcalculator	\
		hm3/diskfile hm3/flusher hm3/idgenerator hm3/tableloader	\
		hm3				\
		hm3/test hm3/main

include $(addsuffix /Makefile.dir, $(SUBDIRS))

clean:
	rm -f \
		$(O)*.o		\
		$(O)*.d		\
		$(EXTRA_CLEAN)

