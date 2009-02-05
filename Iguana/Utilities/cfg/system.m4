dnl +++######################################################################
AC_DEFUN(LAT_CHECK_CADDR_T,
 [AC_MSG_CHECKING(for caddr_t in sys/types.h)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_caddr_t,
   [AC_TRY_COMPILE([#include <sys/types.h>],[caddr_t address = 0;],
     lat_cv_caddr_t=yes, lat_cv_caddr_t=no)])
  AC_MSG_RESULT($lat_cv_caddr_t)
  if test "$lat_cv_caddr_t" = yes; then
    AC_DEFINE(HAVE_CADDR_T)
  fi
  AC_LANG_RESTORE
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_UTSNAME_DOMAINNAME,
 [AC_MSG_CHECKING(for domainname in utsname)
  AC_CACHE_VAL(lat_cv_utsname_domainname,
   [AC_TRY_COMPILE([#include <sys/utsname.h>],[struct utsname n; n.domainname;],
     lat_cv_utsname_domainname=yes, lat_cv_utsname_domainname=no)])
  AC_MSG_RESULT($lat_cv_utsname_domainname)
  if test "$lat_cv_utsname_domainname" = yes; then
    AC_DEFINE(HAVE_UTSNAME_DOMAINNAME)
  fi
])
dnl ---######################################################################
