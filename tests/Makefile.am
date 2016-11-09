AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBFSREFS_DLL_IMPORT@

TESTS = \
	test_api_functions.sh

check_SCRIPTS = \
	test_api_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsrefs_test_error \
	fsrefs_test_support

fsrefs_test_error_SOURCES = \
	fsrefs_test_error.c \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_unused.h

fsrefs_test_error_LDADD = \
	../libfsrefs/libfsrefs.la

fsrefs_test_support_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_support.c \
	fsrefs_test_unused.h

fsrefs_test_support_LDADD = \
	@LIBCERROR_LIBADD@ \
	../libfsrefs/libfsrefs.la

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

