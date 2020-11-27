#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libfsrefs/.libs/libfsrefs.1.dylib && test -f ./pyfsrefs/.libs/pyfsrefs.so;
then
	install_name_tool -change /usr/local/lib/libfsrefs.1.dylib ${PWD}/libfsrefs/.libs/libfsrefs.1.dylib ./pyfsrefs/.libs/pyfsrefs.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

