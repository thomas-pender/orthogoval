ACLOCAL_AMFLAGS = -I m4
SUBDIRS = lib

noinst_LTLIBRARIES = src/libcliquer.la src/libspread.la

src_libcliquer_la_SOURCES = $(top_srcdir)/cliquer-1.21/cliquer.h \
$(top_srcdir)/cliquer-1.21/set.h $(top_srcdir)/cliquer-1.21/graph.h \
$(top_srcdir)/cliquer-1.21/misc.h $(top_srcdir)/cliquer-1.21/reorder.h \
$(top_srcdir)/cliquer-1.21/cliquerconf.h $(top_srcdir)/cliquer-1.21/cl.c \
$(top_srcdir)/cliquer-1.21/cliquer.c $(top_srcdir)/cliquer-1.21/graph.c \
$(top_srcdir)/cliquer-1.21/reorder.c
src_libcliquer_la_CPPFLAGS = -I$(top_srcdir)/cliquer-1.21 -DENABLE_LONG_OPTIONS

src_libspread_la_SOURCES = $(top_srcdir)/src/matrix.h $(top_srcdir)/src/ag.h \
$(top_srcdir)/src/matrix.cpp $(top_srcdir)/src/ag.cpp $(top_srcdir)/src/libspread.h
src_libspread_la_CPPFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = src/mutually_orthogoval src/transversal \
src/automorphisms src/feasible_subspaces src/feasible_coverings

src_mutually_orthogoval_SOURCES = $(top_srcdir)/src/mutually_orthogoval.c
src_mutually_orthogoval_CPPFLAGS = -I$(top_srcdir)/cliquer-1.21 \
-I$(top_srcdir)/lib -I$(top_builddir)/lib
src_mutually_orthogoval_LDADD = $(LTLIBINTL) lib/libgnu.a \
$(top_builddir)/src/libcliquer.la

src_transversal_SOURCES = $(top_srcdir)/src/transversal.cpp
src_transversal_CPPFLAGS = -I$(srcdir)/src
src_transversal_LDADD = $(top_builddir)/src/libspread.la

src_automorphisms_SOURCES = $(top_srcdir)/src/automorphisms.cpp
src_automorphisms_CPPFLAGS = -I$(top_srcdir)/src
src_automorphisms_LDADD = $(top_builddir)/src/libspread.la

src_feasible_subspaces_SOURCES = $(top_srcdir)/src/feasible_subspaces.cpp
src_feasible_subspaces_CPPFLAGS = -I$(top_srcdir)/src
src_feasible_subspaces_LDADD = $(top_builddir)/src/libspread.la

src_feasible_coverings_SOURCES = $(top_srcdir)/src/feasible_coverings.c
src_feasible_coverings_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
-I$(top_srcdir)/src
src_feasible_coverings_LDADD = $(LTLIBINTL) lib/libgnu.a

EXTRA_DIST = $(top_srcdir)/data $(top_srcdir)/massif.4.out \
$(top_srcdir)/massif.8.out README.md m4/gnulib-cache.m4
