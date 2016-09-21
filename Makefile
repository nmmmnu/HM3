#MYCC		= /usr/lib/clang/c++-analyzer
MYCC		= clang
#MYCC		= g++

# ======================================================

EXTRA_CLEAN	:=
EXTRA_INCL	:=

# ======================================================

UNAME		= $(shell uname -s)

CF_DEPS		= -MMD -MP
CF_INCL		= -Iinclude $(EXTRA_INCL)
CF_OPTIM	= -O3
CF_WARN		= -Wall -Wpedantic -Wdeprecated -Wconversion

CF_MISC		=

ifeq ($(UNAME), FreeBSD)
# need for FreeBSD
CF_MISC		= -D_GLIBCXX_USE_C99 -D_GLIBCXX_USE_C99_MATH -D_GLIBCXX_USE_C99_MATH_TR1
endif

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

ifeq ($(UNAME), FreeBSD)
# need for FreeBSD
CF_MISC		= -lm
endif

LINK		= $(MYCC) $(LD_ALL) -o $@ $^ $(LL_ALL)

# ======================================================

A		= bin/
O		= obj/

# ======================================================

include Makefile.dirlist

include $(addsuffix /Makefile.dir, $(SUBDIRS))

-include $(wildcard $(O)*.d)

clean:
	rm -f \
		$(O)*.o		\
		$(O)*.d		\
		$(EXTRA_CLEAN)

paramtest:
	@echo $(wildcard $(O)*.d)

.PHONY: all clean paramtest

