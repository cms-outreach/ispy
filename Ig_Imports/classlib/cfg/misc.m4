dnl +++######################################################################
dnl Stolen from DDD.
AC_DEFUN(LAT_CHECK_DECL,
 [lat_tr=`echo $1 | tr '[[a-z]]' '[[A-Z]]'`
  lat_have_tr=HAVE_$lat_tr
  lat_have_decl_tr=${lat_have_tr}_DECL
  lat_have_$1=no
  AC_CHECK_FUNCS($1, lat_have_$1=yes)
  if test "${lat_have_$1}" = yes; then
    AC_MSG_CHECKING(for $1 declaration in $2)
    AC_CACHE_VAL(lat_cv_have_$1_decl,
     [lat_cv_have_$1_decl=no
      lat_re_params='[[a-zA-Z_]][[a-zA-Z0-9_]]*'
      lat_re_word='(^|[[^a-zA-Z_0-9_]])'
      for header in $2; do
        # Check for ordinary declaration
        AC_EGREP_CPP([${lat_re_word}$1 *\(], [#include <$header>],
          lat_cv_have_$1_decl=yes)
        if test "$lat_cv_have_$1_decl" = yes; then break; fi
        # Check for fixed declaration like "getpid _PARAMS((int))"
        AC_EGREP_CPP([${lat_re_word}$1 *$lat_re_params\(\(],
		     [#include <$header>], lat_cv_have_$1_decl=yes)
        if test "$lat_cv_have_$1_decl" = yes; then break; fi
      done])
    AC_MSG_RESULT($lat_cv_have_$1_decl)
    if test "$lat_cv_have_$1_decl" = yes; then
      AC_DEFINE_UNQUOTED(${lat_have_decl_tr})
    fi
  fi])
dnl ---######################################################################
