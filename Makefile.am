ACLOCAL_AMFLAGS = -I m4

noinst_LTLIBRARIES = src/libcliquer.la src/libspread.la

src_libcliquer_la_SOURCES = $(top_srcdir)/cliquer-1.21/cliquer.h \
$(top_srcdir)/cliquer-1.21/set.h $(top_srcdir)/cliquer-1.21/graph.h \
$(top_srcdir)/cliquer-1.21/misc.h $(top_srcdir)/cliquer-1.21/reorder.h \
$(top_srcdir)/cliquer-1.21/cliquerconf.h $(top_srcdir)/cliquer-1.21/cl.c \
$(top_srcdir)/cliquer-1.21/cliquer.c $(top_srcdir)/cliquer-1.21/graph.c \
$(top_srcdir)/cliquer-1.21/reorder.c
src_libcliquer_la_CPPFLAGS = -I$(top_srcdir)/cliquer-1.21 -DENABLE_LONG_OPTIONS

src_libspread_la_SOURCES = $(top_srcdir)/src/matrix.h $(top_srcdir)/src/ag.h \
$(top_srcdir)/src/matrix.cpp $(top_srcdir)/src/ag.cpp
src_libspread_la_CPPFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = src/mutually_orthogoval src/raw_search src/transversal \
src/automorphisms

src_mutually_orthogoval_SOURCES = $(top_srcdir)/src/mutually_orthogoval.c
src_mutually_orthogoval_CPPFLAGS = -I$(top_srcdir)/cliquer-1.21
src_mutually_orthogoval_LDADD = $(top_builddir)/src/libcliquer.la

src_raw_search_SOURCES = $(top_srcdir)/src/raw_search.cpp
src_raw_search_CPPFLAGS = -I$(top_srcdir)/src
src_raw_search_LDADD = $(top_builddir)/src/libspread.la

src_transversal_SOURCES = $(top_srcdir)/src/transversal.cpp
src_transversal_CPPFLAGS = -I$(srcdir)/src
src_transversal_LDADD = $(top_builddir)/src/libspread.la

src_automorphisms_SOURCES = $(top_srcdir)/src/automorphisms.cpp
src_automorphisms_CPPFLAGS = -I$(top_srcdir)/src
src_automorphisms_LDADD = $(top_builddir)/src/libspread.la

EXTRA_DIST = $(top_srcdir)/data $(top_srcdir)/massif.4.out \
$(top_srcdir)/massif.8.out README.md
