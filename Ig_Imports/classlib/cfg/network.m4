dnl +++######################################################################
AC_DEFUN(LAT_CHECK_FD_SET,
 [AC_MSG_CHECKING(for fd_set)
  AC_CACHE_VAL(lat_cv_check_fd_set,
   [lat_cv_check_fd_set=''
    AC_TRY_COMPILE([
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif],
    [fd_set set;], lat_cv_check_fd_set=yes, lat_cv_check_fd_set=no)])
  AC_MSG_RESULT($lat_cv_check_fd_set)
  if test $lat_cv_check_fd_set = no; then
    AC_DEFINE_UNQUOTED(fd_set,int)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_FUNC_SELECT_ARG_TYPE,
 [AC_MSG_CHECKING(for select argument type)
  AC_CACHE_VAL(lat_cv_func_select_arg_type,
   [lat_cv_func_select_arg_type=''
    AC_TRY_COMPILE([
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <string.h>],
[fd_set s; FD_ZERO(&s);select (0, &s, 0, 0, 0);],
    lat_cv_func_select_arg_type='fd_set*',
    lat_cv_func_select_arg_type='int*')])
  AC_MSG_RESULT($lat_cv_func_select_arg_type)
  AC_DEFINE_UNQUOTED(SELECT_ARG_TYPE,[$lat_cv_func_select_arg_type])])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SOCKADDR_UN,
 [AC_MSG_CHECKING(for sockaddr_un)
  AC_CACHE_VAL(lat_cv_check_sockaddr_un,
   [lat_cv_check_sockaddr_un=''
    AC_TRY_COMPILE([
#ifdef _WIN32
#include <windows.h>
#else
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#endif],
    [struct sockaddr_un addr;],
    lat_cv_check_sockaddr_un=yes,
    lat_cv_check_sockaddr_un=no)])
  AC_MSG_RESULT($lat_cv_check_sockaddr_un)
  if test $lat_cv_check_sockaddr_un = yes ; then
    AC_DEFINE(HAVE_SOCKADDR_UN)
  fi])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_FUNC_RECV_ARG_TYPE,
 [AC_MSG_CHECKING(for recv argument type)
  AC_CACHE_VAL(lat_cv_func_recv_arg_type,
   [lat_cv_func_recv_arg_type=''
    AC_TRY_COMPILE([
#ifdef _WIN32
#include <windows.h>
#else
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#endif],
    [recv (0, (void *)0, 0, 0);],
    lat_cv_func_recv_arg_type='void *',
    lat_cv_func_recv_arg_type='char *')])
  AC_MSG_RESULT($lat_cv_func_recv_arg_type)
  AC_DEFINE_UNQUOTED(RECV_ARG_TYPE,[$lat_cv_func_recv_arg_type])])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_FUNC_GETSOCKOPT_LEN_TYPE,
 [AC_MSG_CHECKING(for getsockopt argument type)
  AC_CACHE_VAL(lat_cv_func_getsockopt_len_type,
   [lat_cv_func_getsockopt_len_type=''
    for lat_ac_type in socklen_t size_t int; do
      AC_TRY_COMPILE([
#ifdef _WIN32
#include <windows.h>
#else
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#endif],
      [$lat_ac_type s; getsockopt (0, 0, 0, 0, &s);],
      [lat_cv_func_getsockopt_len_type=$lat_ac_type; break;])
    done])
  AC_MSG_RESULT($lat_cv_func_getsockopt_len_type)
  AC_DEFINE_UNQUOTED(SOCKOPT_LEN_TYPE,[$lat_cv_func_getsockopt_len_type])])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SOCKADDR_LENGTH_TYPE,
 [AC_MSG_CHECKING(for socket functions' length argument type)
  AC_CACHE_VAL(lat_cv_check_sockaddr_length_type,
   [lat_cv_check_sockaddr_length_type=''
    for lat_ac_type in socklen_t size_t int; do
       AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef _WIN32
#include <windows.h>
#elif HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif],[struct sockaddr *from; $lat_ac_type len = 0; accept (0, from, &len);],
        [lat_cv_check_sockaddr_length_type=$lat_ac_type; break;])
    done])
   AC_MSG_RESULT($lat_cv_check_sockaddr_length_type)
   AC_DEFINE_UNQUOTED(SOCKET_LEN_TYPE,[$lat_cv_check_sockaddr_length_type])])
dnl ---######################################################################
