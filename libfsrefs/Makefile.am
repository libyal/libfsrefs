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
	@LIBFSREFS_DLL_EXPORT@

lib_LTLIBRARIES = libfsrefs.la

libfsrefs_la_SOURCES = \
	fsrefs_index.h \
	fsrefs_metadata_block.h \
	fsrefs_volume_header.h \
	fsrefs_volume_information.h \
	libfsrefs.c \
	libfsrefs_debug.c libfsrefs_debug.h \
	libfsrefs_definitions.h \
	libfsrefs_directory.c libfsrefs_directory.h \
	libfsrefs_block_descriptor.c libfsrefs_block_descriptor.h \
	libfsrefs_block_descriptors.c libfsrefs_block_descriptors.h \
	libfsrefs_error.c libfsrefs_error.h \
	libfsrefs_extern.h \
	libfsrefs_io_handle.c libfsrefs_io_handle.h \
	libfsrefs_level0_metadata.c libfsrefs_level0_metadata.h \
	libfsrefs_level1_metadata.c libfsrefs_level1_metadata.h \
	libfsrefs_level2_metadata.c libfsrefs_level2_metadata.h \
	libfsrefs_level3_metadata.c libfsrefs_level3_metadata.h \
	libfsrefs_libbfio.h \
	libfsrefs_libcdata.h \
	libfsrefs_libcerror.h \
	libfsrefs_libclocale.h \
	libfsrefs_libcnotify.h \
	libfsrefs_libfcache.h \
	libfsrefs_libfdata.h \
	libfsrefs_libfdatetime.h \
	libfsrefs_libfguid.h \
	libfsrefs_libfwnt.h \
	libfsrefs_libuna.h \
	libfsrefs_metadata_block.c libfsrefs_metadata_block.h \
	libfsrefs_metadata_table.c libfsrefs_metadata_table.h \
	libfsrefs_metadata_value.c libfsrefs_metadata_value.h \
	libfsrefs_notify.c libfsrefs_notify.h \
	libfsrefs_support.c libfsrefs_support.h \
	libfsrefs_types.h \
	libfsrefs_unused.h \
	libfsrefs_volume.c libfsrefs_volume.h \
	libfsrefs_volume_header.c libfsrefs_volume_header.h \
	libfsrefs_volume_information.c libfsrefs_volume_information.h \
	libfsrefs_volume_name.c libfsrefs_volume_name.h

libfsrefs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFWNT_LIBADD@

libfsrefs_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfsrefs_definitions.h.in \
	libfsrefs.rc \
	libfsrefs.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfsrefs_definitions.h
	/bin/rm -f libfsrefs.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libfsrefs ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfsrefs_la_SOURCES)

