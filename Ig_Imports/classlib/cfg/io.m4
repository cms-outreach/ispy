dnl +++######################################################################
AC_DEFUN(LAT_CHECK_SENDFILE,
 [AC_CHECK_LIB(sendfile, sendfile,
   [# FIXME: LIBS="$LIBS -lsendfile"
    AC_DEFINE(HAVE_SOLARIS_SENDFILE, 1,
      [Define if you have solaris-compatible sendfile()])],
   [AC_MSG_CHECKING(for linux-compatible sendfile())
    AC_TRY_COMPILE([
        #undef _FILE_OFFSET_BITS
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <sys/sendfile.h>],
      [off_t x; sendfile (0, 0, &x, 0);],
      [AC_DEFINE(HAVE_LINUX_SENDFILE, 1,
	[Define if you have linux-compatible sendfile()])
       AC_MSG_RESULT(yes)],
      [AC_MSG_RESULT(no)])

    AC_MSG_CHECKING(for freebsd-compatible sendfile())
    AC_TRY_COMPILE([
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <sys/uio.h>],
      [struct sf_hdtr hdtr; sendfile (0, 0, 0, 0, &hdtr, (void *) 0, 0);],
      [AC_DEFINE(HAVE_FREEBSD_SENDFILE, 1,
	[Define if you have freebsd-compatible sendfile()])
       AC_MSG_RESULT(yes)],
      [AC_MSG_RESULT(no)])
  ])])
dnl ---######################################################################
