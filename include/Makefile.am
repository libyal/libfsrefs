include_HEADERS = \
	libfsrefs.h

pkginclude_HEADERS = \
	libfsrefs/codepage.h \
	libfsrefs/definitions.h \
	libfsrefs/error.h \
	libfsrefs/extern.h \
	libfsrefs/features.h \
	libfsrefs/types.h

EXTRA_DIST = \
	libfsrefs.h.in \
	libfsrefs/definitions.h.in \
	libfsrefs/features.h.in \
	libfsrefs/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfsrefs.h
	/bin/rm -f libfsrefs/definitions.h
	/bin/rm -f libfsrefs/features.h
	/bin/rm -f libfsrefs/types.h
	/bin/rm -f Makefile

