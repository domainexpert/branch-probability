# Copyright 2017 National University of Singapore
#
# See LICENSE.md for license information.

CC=clang
CXX=clang++
CXXFLAGS=`llvm-config --cxxflags` -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
LDFLAGS=`llvm-config --ldflags`
LDLIBS=`llvm-config --libs` -lpthread -lncurses -ldl
EXAMPLES=

TARGETS=PrintBranchProbability ${EXAMPLES}

all: ${TARGETS}

clean:
	rm -f ${TARGETS} *.bc *.ll

.SUFFIXES: .bc .ll

.c.bc:
	${CC} -c -emit-llvm -g -o $*_123.bc $<
	opt -mem2reg < $*_123.bc > $@
	rm -f $*_123.bc

.bc.ll:
	llvm-dis $<
