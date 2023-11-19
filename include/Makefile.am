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
	-rm -f libfsrefs.h
	-rm -f libfsrefs/definitions.h
	-rm -f libfsrefs/features.h
	-rm -f libfsrefs/types.h
	-rm -f Makefile

