#MYCC		= /usr/lib/clang/c++-analyzer
MYCC		= clang++
#MYCC		= g++

# ======================================================

EXTRA_CLEAN	:=
EXTRA_INCL	:=

# ======================================================

UNAME		= $(shell uname -s)

# ======================================================

CF_DEPS		= -MMD -MP
CF_INCL		= -Iinclude $(EXTRA_INCL)
CF_OPTIM	= -O2
#CF_OPTIM	+= -march=native
CF_WARN		= -Wall -Wpedantic -Wdeprecated -Wconversion

CF_MISC		=

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

# ======================================================

A		= bin/
O		= obj/

# ======================================================

ifeq ($(UNAME), FreeBSD)
# add correct endian for FreeBSD
# fix compilation for FreeBSD
EXTRA_INCL	+= -Iinclude.freebsd/
CF_MISC		+= -D_GLIBCXX_USE_C99 -D_GLIBCXX_USE_C99_MATH -D_GLIBCXX_USE_C99_MATH_TR1
LL_ALL		+= -lm
endif

# ======================================================

ifeq ($(UNAME), Darwin)
# fix endian mess for OSX
EXTRA_INCL	+= -Iinclude.darwin/
endif

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

