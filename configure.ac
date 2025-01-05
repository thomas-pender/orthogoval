AC_PREREQ([2.71])
AC_INIT([orthogoval], [0.0])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/feasible_subspaces.cpp])
AC_CONFIG_HEADERS([src/config.h])

AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])

: ${CFLAGS="-O3"}
AC_PROG_CC
gl_EARLY

AM_PROG_AR

LT_PREREQ([2.4.6])
LT_INIT([static disable-shared])

#-------------------------------------------------
# GNU C++
#-------------------------------------------------
: ${CXXFLAGS="-O3"}
AC_PROG_CXX
if test x${GXX} = xyes; then
AC_DEFINE([HAVE_GXX], [1], [GNU C++ enabled])
fi
#-------------------------------------------------

#-------------------------------------------------
# C++20
#-------------------------------------------------
AX_CXX_COMPILE_STDCXX([20])
#-------------------------------------------------

gl_INIT

AC_CONFIG_FILES([Makefile lib/Makefile doc/Makefile])

AC_OUTPUT

cat << EOF

============================================================

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

C compiler  : '${CC} ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} ${LIBS}'
CXX compiler: '${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} ${LIBS}'

Compile with 'make'.

============================================================

EOF
