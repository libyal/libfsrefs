AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
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
	test_library.sh \
	test_tools.sh \
	test_fsrefsinfo.sh

check_SCRIPTS = \
	test_fsrefsinfo.sh \
	test_library.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsrefs_test_attribute_values \
	fsrefs_test_block_descriptor \
	fsrefs_test_block_reference \
	fsrefs_test_block_tree \
	fsrefs_test_block_tree_node \
	fsrefs_test_checkpoint \
	fsrefs_test_data_run \
	fsrefs_test_directory_entry \
	fsrefs_test_directory_object \
	fsrefs_test_error \
	fsrefs_test_file_entry \
	fsrefs_test_file_system \
	fsrefs_test_io_handle \
	fsrefs_test_metadata_block_header \
	fsrefs_test_ministore_node \
	fsrefs_test_node_header \
	fsrefs_test_node_record \
	fsrefs_test_notify \
	fsrefs_test_objects_tree \
	fsrefs_test_superblock \
	fsrefs_test_support \
	fsrefs_test_tools_info_handle \
	fsrefs_test_tools_output \
	fsrefs_test_tools_signal \
	fsrefs_test_tree_header \
	fsrefs_test_volume \
	fsrefs_test_volume_header

fsrefs_test_attribute_values_SOURCES = \
	fsrefs_test_attribute_values.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_attribute_values_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

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

fsrefs_test_block_reference_SOURCES = \
	fsrefs_test_block_reference.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_block_reference_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_block_tree_SOURCES = \
	fsrefs_test_block_tree.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_block_tree_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_block_tree_node_SOURCES = \
	fsrefs_test_block_tree_node.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_block_tree_node_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_checkpoint_SOURCES = \
	fsrefs_test_checkpoint.c \
	fsrefs_test_functions.c fsrefs_test_functions.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_checkpoint_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_data_run_SOURCES = \
	fsrefs_test_data_run.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_data_run_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_directory_entry_SOURCES = \
	fsrefs_test_directory_entry.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_directory_entry_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_directory_object_SOURCES = \
	fsrefs_test_directory_object.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_directory_object_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_error_SOURCES = \
	fsrefs_test_error.c \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_unused.h

fsrefs_test_error_LDADD = \
	../libfsrefs/libfsrefs.la

fsrefs_test_file_entry_SOURCES = \
	fsrefs_test_file_entry.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_file_entry_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fsrefs_test_file_system_SOURCES = \
	fsrefs_test_file_system.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_file_system_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

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

fsrefs_test_metadata_block_header_SOURCES = \
	fsrefs_test_functions.c fsrefs_test_functions.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_metadata_block_header.c \
	fsrefs_test_unused.h

fsrefs_test_metadata_block_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_ministore_node_SOURCES = \
	fsrefs_test_functions.c fsrefs_test_functions.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_ministore_node.c \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h

fsrefs_test_ministore_node_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_node_header_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_node_header.c \
	fsrefs_test_unused.h

fsrefs_test_node_header_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_node_record_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_node_record.c \
	fsrefs_test_unused.h

fsrefs_test_node_record_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_notify_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_notify.c \
	fsrefs_test_unused.h

fsrefs_test_notify_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_objects_tree_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_objects_tree.c \
	fsrefs_test_unused.h

fsrefs_test_objects_tree_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_superblock_SOURCES = \
	fsrefs_test_functions.c fsrefs_test_functions.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_superblock.c \
	fsrefs_test_unused.h

fsrefs_test_superblock_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_support_SOURCES = \
	fsrefs_test_functions.c fsrefs_test_functions.h \
	fsrefs_test_getopt.c fsrefs_test_getopt.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libclocale.h \
	fsrefs_test_libcnotify.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_libuna.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_support.c \
	fsrefs_test_unused.h

fsrefs_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_tools_info_handle_SOURCES = \
	../fsrefstools/info_handle.c ../fsrefstools/info_handle.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_tools_info_handle.c \
	fsrefs_test_unused.h

fsrefs_test_tools_info_handle_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_tools_output_SOURCES = \
	../fsrefstools/fsrefstools_output.c ../fsrefstools/fsrefstools_output.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_macros.h \
	fsrefs_test_tools_output.c \
	fsrefs_test_unused.h

fsrefs_test_tools_output_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_tools_signal_SOURCES = \
	../fsrefstools/fsrefstools_signal.c ../fsrefstools/fsrefstools_signal.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_macros.h \
	fsrefs_test_tools_signal.c \
	fsrefs_test_unused.h

fsrefs_test_tools_signal_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_tree_header_SOURCES = \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_tree_header.c \
	fsrefs_test_unused.h

fsrefs_test_tree_header_LDADD = \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

fsrefs_test_volume_SOURCES = \
	fsrefs_test_functions.c fsrefs_test_functions.h \
	fsrefs_test_getopt.c fsrefs_test_getopt.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libclocale.h \
	fsrefs_test_libcnotify.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_libuna.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h \
	fsrefs_test_volume.c

fsrefs_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fsrefs_test_volume_header_SOURCES = \
	fsrefs_test_functions.c fsrefs_test_functions.h \
	fsrefs_test_libbfio.h \
	fsrefs_test_libcerror.h \
	fsrefs_test_libfsrefs.h \
	fsrefs_test_macros.h \
	fsrefs_test_memory.c fsrefs_test_memory.h \
	fsrefs_test_unused.h \
	fsrefs_test_volume_header.c

fsrefs_test_volume_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsrefs/libfsrefs.la \
	@LIBCERROR_LIBADD@

DISTCLEANFILES = \
	Makefile \
	Makefile.in \
	notify_stream.log

