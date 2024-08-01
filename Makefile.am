VPATH = $(srcdir):$(builddir)

noinst_LIBRARIES = src/libcliquer.a src/libspread.a

src_libcliquer_a_SOURCES = cliquer-1.21/cliquer.h \
cliquer-1.21/set.h cliquer-1.21/graph.h cliquer-1.21/misc.h cliquer-1.21/reorder.h cliquer-1.21/cliquerconf.h \
cliquer-1.21/cl.c cliquer-1.21/cliquer.c cliquer-1.21/graph.c cliquer-1.21/reorder.c
src_libcliquer_a_CPPFLAGS = -I$(srcdir)/cliquer-1.21 -DENABLE_LONG_OPTIONS

src_libspread_a_SOURCES = src/matrix.h src/ag.h src/matrix.cpp src/ag.cpp
src_libspread_a_CPPFLAGS = -I$(srcdir)/src

noinst_PROGRAMS = src/mutually_orthogoval src/raw_search src/transversal

src_mutually_orthogoval_SOURCES = src/mutually_orthogoval.c
src_mutually_orthogoval_CPPFLAGS = -I$(srcdir)/cliquer-1.21
src_mutually_orthogoval_LDADD = src/libcliquer.a

src_raw_search_SOURCES = src/raw_search.cpp
src_raw_search_CPPFLAGS = -I$(srcdir)/src
src_raw_search_LDADD = src/libspread.a

src_transversal_SOURCES = src/transversal.cpp
src_transversal_CPPFLAGS = -I$(srcdir)/src
src_transversal_LDADD = src/libspread.a

EXTRA_DIST = $(srcdir)/var $(srcdir)/massif.4.out $(srcdir)/massif.8.out README.md
