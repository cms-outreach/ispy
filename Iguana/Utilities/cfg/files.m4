dnl +++######################################################################
AC_DEFUN(LAT_CHECK_F_SETSIG,
 [AC_MSG_CHECKING(for F_SETSIG)
  AC_CACHE_VAL(lat_cv_check_f_setsig,
   [AC_EGREP_CPP(YUP_HAVE_F_SETSIG, 
     [#include <fcntl.h>
      #ifdef F_SETSIG
       YUP_HAVE_F_SETSIG
      #endif],
      lat_cv_check_f_setsig=yes, lat_cv_check_f_setsig=no)])
  AC_MSG_RESULT($lat_cv_check_f_setsig)
  if test $lat_cv_check_f_setsig = yes; then
    AC_DEFINE(HAVE_F_SETSIG)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_F_SETAUXFL,
 [AC_MSG_CHECKING(for F_SETAUXFL)
  AC_CACHE_VAL(lat_cv_check_f_setauxfl,
   [AC_EGREP_CPP(YUP_HAVE_F_SETAUXFL, 
     [#include <fcntl.h>
      #ifdef F_SETAUXFL
        YUP_HAVE_F_SETAUXFL
      #endif],
      lat_cv_check_f_setauxfl=yes, lat_cv_check_f_setauxfl=no)])
  AC_MSG_RESULT($lat_cv_check_f_setauxfl)
  if test $lat_cv_check_f_setauxfl = yes; then
    AC_DEFINE(HAVE_F_SETAUXFL)
  fi])
dnl ---######################################################################
