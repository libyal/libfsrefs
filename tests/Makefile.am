AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	fsrefs_test_get_version

fsrefs_test_get_version_SOURCES = \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_libcstring.h \
	fsrefs_test_get_version.c \
	fsrefs_test_unused.h

fsrefs_test_get_version_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

