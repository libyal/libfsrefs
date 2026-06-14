#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libfsrefs/.libs/libfsrefs.1.dylib && test -f ./pyfsrefs/.libs/pyfsrefs.so
then
	install_name_tool -change /usr/local/lib/libfsrefs.1.dylib ${PWD}/libfsrefs/.libs/libfsrefs.1.dylib ./pyfsrefs/.libs/pyfsrefs.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

