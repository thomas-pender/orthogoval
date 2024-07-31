AC_PREREQ([2.71])
AC_INIT([orthogoval], [0.0])

dnl AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/raw_search.cpp])
AC_CONFIG_HEADERS([src/config.h])

AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])

AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

#-------------------------------------------------
# GNU C++
#-------------------------------------------------
AC_PROG_CXX
if test x${GXX} = xyes; then
AC_DEFINE([HAVE_GXX], [1], [GNU C++ enabled])
fi
#-------------------------------------------------

#-------------------------------------------------
# asynchronous execution
#-------------------------------------------------
AC_ARG_ENABLE([async-exec],
[AS_HELP_STRING([--disable-async-exec],
[disable asynchronous execution @<:@default: no@:>@])],
[async_exec=${enableval}], [async_exec=yes])

if test "x${async_exec}" = xyes; then
AX_PTHREAD
CXX=$PTHREAD_CXX
LIBS="$PTHREAD_LIBS $LIBS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
AC_DEFINE([ASYNC_EXEC], [1], [asynchronous execution enabled])
fi
#-------------------------------------------------

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

cat << EOF

============================================================

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

C compiler  : '${CC} ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} ${LIBS}'
CXX compiler: '${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} ${LIBS}'

Package features:
- asynchronous execution: ${async_exec}

Compile with 'make'.

============================================================

EOF
