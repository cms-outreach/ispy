dnl +++######################################################################
dnl Stolen from DDD.
AC_DEFUN(LAT_CHECK_TYPE_SIGNAL,
 [AC_MSG_CHECKING(return type of signal handlers)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_type_signal,
   [AC_TRY_COMPILE([
#include <sys/types.h>
#include <signal.h>
extern "C" void handler (int signalnr);],
     [signal(1, handler);],
     lat_cv_type_signal=void,
     lat_cv_type_signal=int)])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_type_signal)
  AC_DEFINE_UNQUOTED(RETTYPE_SIGHANDLER,[$lat_cv_type_signal])])
dnl ---######################################################################
dnl +++######################################################################
dnl Stolen from DDD.
AC_DEFUN(LAT_CHECK_TYPE_SIGNAL_PROC,
 [AC_MSG_CHECKING(parameter type of signal handlers)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_type_sighandler_args,
   [AC_TRY_COMPILE([
#include <sys/types.h>
#include <signal.h>
extern "C" RETTYPE_SIGHANDLER handler (int);],
     [signal(1, handler);], [lat_cv_type_sighandler_args="int"],
     [AC_TRY_COMPILE([
#include <sys/types.h>
#include <signal.h>
extern "C" RETTYPE_SIGHANDLER handler (int, ...);],
       [signal(1, handler);], [lat_cv_type_sighandler_args="int, ..."],
       [AC_TRY_COMPILE([
#include <sys/types.h>
#include <signal.h>
extern "C" RETTYPE_SIGHANDLER handler (...);],
         [signal(1, handler);], [lat_cv_type_sighandler_args="..."],
	 [AC_MSG_WARN([You need to define the ARGS_SIGHANDLER macro in config.h])])])])])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_type_sighandler_args)
  AC_DEFINE_UNQUOTED(ARGS_SIGHANDLER,[$lat_cv_type_sighandler_args])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_POSIX_SIGNALS,
 [AC_MSG_CHECKING(for POSIX signals)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_check_posix_signals,
   [AC_TRY_COMPILE([#include <signal.h>],
     [struct sigaction a; sigemptyset(&a.sa_mask); sigaction(1, &a, &a);],
     lat_cv_check_posix_signals=yes,
     lat_cv_check_posix_signals=no)])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_check_posix_signals)
  if test $lat_cv_check_posix_signals = yes ; then
    AC_DEFINE(HAVE_POSIX_SIGNALS)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_POSIX_RT_SIGNALS,
 [AC_MSG_CHECKING(for POSIX real-time signals)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_check_posix_rt_signals,
   [AC_TRY_COMPILE([#include <signal.h>],
     [union sigval v; sigqueue(0, 0, v); sigtimedwait (0, 0, 0);],
     lat_cv_check_posix_rt_signals=yes,
     lat_cv_check_posix_rt_signals=no)])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_check_posix_rt_signals)
  if test $lat_cv_check_posix_rt_signals = yes ; then
    AC_DEFINE(HAVE_POSIX_RT_SIGNALS)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SYS_SIGLIST,
 [AC_MSG_CHECKING(for sys_siglist)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_check_sys_siglist,
   [AC_TRY_COMPILE([#include <signal.h>
	#include <stdio.h>],
     [puts(sys_siglist [SIGINT]);],
     lat_cv_check_sys_siglist=yes,
     lat_cv_check_sys_siglist=no)])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_check_sys_siglist)
  if test $lat_cv_check_sys_siglist = yes ; then
    AC_DEFINE(HAVE_SYS_SIGLIST)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SIGINFO_SI_UTIME,
 [AC_MSG_CHECKING(for siginfo_t member si_utime)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_check_siginfo_si_utime,
   [AC_TRY_COMPILE([#include <signal.h>],
     [siginfo_t i; i.si_utime;],
     lat_cv_check_siginfo_si_utime=yes,
     lat_cv_check_siginfo_si_utime=no)])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_check_siginfo_si_utime)
  if test $lat_cv_check_siginfo_si_utime = yes ; then
    AC_DEFINE(HAVE_SIGINFO_SI_UTIME)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SIGINFO_SI_INT,
 [AC_MSG_CHECKING(for siginfo_t member si_int)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_check_siginfo_si_int,
   [AC_TRY_COMPILE([#include <signal.h>],
     [siginfo_t i; i.si_int;],
     lat_cv_check_siginfo_si_int=yes,
     lat_cv_check_siginfo_si_int=no)])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_check_siginfo_si_int)
  if test $lat_cv_check_siginfo_si_int = yes ; then
    AC_DEFINE(HAVE_SIGINFO_SI_INT)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SIGINFO_SI_VALUE,
 [AC_MSG_CHECKING(for siginfo_t member si_value)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_CACHE_VAL(lat_cv_check_siginfo_si_value,
   [AC_TRY_COMPILE([#include <signal.h>],
     [siginfo_t i; i.si_value.sigval_int;],
     lat_cv_check_siginfo_si_value=yes,
     lat_cv_check_siginfo_si_value=no)])
  AC_LANG_RESTORE
  AC_MSG_RESULT($lat_cv_check_siginfo_si_value)
  if test $lat_cv_check_siginfo_si_value = yes ; then
    AC_DEFINE(HAVE_SIGINFO_SI_VALUE)
  fi])
dnl ---######################################################################
