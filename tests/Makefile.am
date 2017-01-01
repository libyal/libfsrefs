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

TESTS = \
	test_api_functions.sh \
	test_api_types.sh

check_SCRIPTS = \
	test_api_functions.sh \
	test_api_types.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsrefs_test_block_descriptor \
	fsrefs_test_block_descriptors \
	fsrefs_test_directory \
	fsrefs_test_error \
	fsrefs_test_io_handle \
	fsrefs_test_level0_metadata \
	fsrefs_test_level1_metadata \
	fsrefs_test_level2_metadata \
	fsrefs_test_level3_metadata \
	fsrefs_test_metadata_block \
	fsrefs_test_metadata_table \
	fsrefs_test_metadata_value \
	fsrefs_test_notify \
	fsrefs_test_support \
	fsrefs_test_volume \
	fsrefs_test_volume_name

fsrefs_test_block_descriptor_SOURCES = \
	fsrefs_test_block_descriptor.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_block_descriptor_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_block_descriptors_SOURCES = \
	fsrefs_test_block_descriptors.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_block_descriptors_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_directory_SOURCES = \
	fsrefs_test_directory.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_directory_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_error_SOURCES = \
	fsrefs_test_error.c \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_unused.h

fsrefs_test_error_LDADD = \
	../libfsrefs/libfsrefs.la

fsrefs_test_io_handle_SOURCES = \
	fsrefs_test_io_handle.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_io_handle_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_level0_metadata_SOURCES = \
	fsrefs_test_level0_metadata.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_level0_metadata_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_level1_metadata_SOURCES = \
	fsrefs_test_level1_metadata.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_level1_metadata_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_level2_metadata_SOURCES = \
	fsrefs_test_level2_metadata.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_level2_metadata_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_level3_metadata_SOURCES = \
	fsrefs_test_level3_metadata.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_level3_metadata_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_metadata_block_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_metadata_block.c \
	fsrefs_test_unused.h

fsrefs_test_metadata_block_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_metadata_table_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_metadata_table.c \
	fsrefs_test_unused.h

fsrefs_test_metadata_table_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_metadata_value_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_metadata_value.c \
	fsrefs_test_unused.h

fsrefs_test_metadata_value_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_notify_SOURCES = \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_notify.c \
	fsrefs_test_unused.h

fsrefs_test_notify_LDADD = \
	../libfsrefs/libfsrefs.la

fsrefs_test_support_SOURCES = \
	fsrefs_test_getopt.c fsrefs_test_getopt.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libclocale.h \
	fsrefs_test_libcnotify.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_libuna.h \
	fsrefs_test_macros.h \
	fsrefs_test_support.c

fsrefs_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_volume_SOURCES = \
	fsrefs_test_getopt.c fsrefs_test_getopt.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libclocale.h \
	fsrefs_test_libcnotify.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_libuna.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_volume.c

fsrefs_test_volume_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fsrefs_test_volume_name_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h \
	fsrefs_test_volume_name.c

fsrefs_test_volume_name_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

