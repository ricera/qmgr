# Use system compiler?
PROG_CXX = qmgr_test
# Test runenr
SRCS += tests_main.cpp
# Test files
SRCS += qmgr_test.cpp
# Implementation files
SRCS += ie_qmgr.h ie_qmgr.c
MK_MAN = no

CPPUTEST_INCLUDE = /usr/local/include
CPPUTEST_LIB = /usr/local/lib

CFLAGS += -I$(CPPUTEST_INCLUDE)
# Include code coverage info for profiling
CFLAGS += -instr-profile --coverage -O0
## Include memory leak detection (for malloc)
# CFLAGS += -include $(CPPUTEST_INCLUDE)/CppUTest/MemoryLeakDetectorMallocMacros.h
LDADD += -L$(CPPUTEST_LIB) -lCppUTest

test: qmgr_test
	@./qmgr_test

coverage: test
	llvm-cov50 gcov qmgr_test.cpp

cleanall: clean cleandepend
	rm -f *.gcda *.gcno *.gcov

.include <bsd.prog.mk>

