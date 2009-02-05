dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SC_OPEN_MAX,
 [AC_MSG_CHECKING(for sysconf(_SC_OPEN_MAX))
  AC_CACHE_VAL(lat_cv_sc_open_max,
   [AC_TRY_COMPILE([#include <unistd.h>],[sysconf (_SC_OPEN_MAX);],
     lat_cv_sc_open_max=yes, lat_cv_sc_open_max=no)])
  AC_MSG_RESULT($lat_cv_sc_open_max)
  if test "$lat_cv_sc_open_max" = yes; then
    AC_DEFINE(HAVE_SC_OPEN_MAX)
  fi
  AC_LANG_RESTORE
])
dnl ---######################################################################
