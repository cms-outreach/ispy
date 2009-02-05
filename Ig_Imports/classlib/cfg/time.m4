dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TM_GMTOFF,
 [AC_MSG_CHECKING(for tm_gmtoff member in tm)
  AC_CACHE_VAL(lat_cv_check_tm_gmtoff,
   [lat_cv_check_tm_gmtoff=''
    AC_TRY_COMPILE([
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif],
    [struct tm t; t.tm_gmtoff;],
    lat_cv_check_tm_gmtoff=yes, lat_cv_check_tm_gmtoff=no)])
  AC_MSG_RESULT($lat_cv_check_tm_gmtoff)
  if test $lat_cv_check_tm_gmtoff = yes; then
    AC_DEFINE(HAVE_TM_GMTOFF)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_TM_ZONE,
 [AC_MSG_CHECKING(for tm_zone member in tm)
  AC_CACHE_VAL(lat_cv_check_tm_zone,
   [lat_cv_check_tm_zone=''
    AC_TRY_COMPILE([
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif],
    [struct tm t; t.tm_zone;],
    lat_cv_check_tm_zone=yes, lat_cv_check_tm_zone=no)])
  AC_MSG_RESULT($lat_cv_check_tm_zone)
  if test $lat_cv_check_tm_zone = yes; then
    AC_DEFINE(HAVE_TM_ZONE)
  fi])
dnl ---######################################################################
