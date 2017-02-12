AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFSREFS_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fsrefsinfo

fsrefsinfo_SOURCES = \
	fsrefsinfo.c \
	fsrefstools_getopt.c fsrefstools_getopt.h \
	fsrefstools_i18n.h \
	fsrefstools_libbfio.h \
	fsrefstools_libcerror.h \
	fsrefstools_libclocale.h \
	fsrefstools_libcnotify.h \
	fsrefstools_libfcache.h \
	fsrefstools_libfdata.h \
	fsrefstools_libfdatetime.h \
	fsrefstools_libfguid.h \
	fsrefstools_libfsrefs.h \
	fsrefstools_libfwnt.h \
	fsrefstools_libuna.h \
	fsrefstools_output.c fsrefstools_output.h \
	fsrefstools_signal.c fsrefstools_signal.h \
	fsrefstools_unused.h \
	info_handle.c info_handle.h

fsrefsinfo_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on fsrefsinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fsrefsinfo_SOURCES)

