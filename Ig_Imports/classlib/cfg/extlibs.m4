dnl +++######################################################################
AC_DEFUN(LAT_EXT_ZLIB,
 [ZINC="-I$srcdir/ext/zlib" ZLTLIB=libext-zlib.la ZLIB=
  AC_SUBST(ZINC)
  AC_SUBST(ZLIB)
  AC_SUBST(ZLTLIB)
  AC_ARG_WITH(zlib,
    [   --with-zlib                    Use separate libz],
    [ with_zlib_includes="${with_zlib_includes-$withval}"
      with_zlib_libraries="${with_zlib_libraries-$withval}" ])
  AC_ARG_WITH(zlib-includes,
    [   --with-zlib-includes=DIR       ZLIB headers are in DIR],
    [ zlib_includes="$withval"
      case "$zlib_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(zlib.h,[ZINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$zlib_includes"
	      AC_CHECK_HEADER(zlib.h,[ZINC="-I$zlib_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(zlib-libraries,
    [   --with-zlib-libaries=DIR       ZLIB libraries are in DIR],
    [ zlib_libraries="$withval"
      case "$zlib_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(zlib.h,
	       [AC_CHECK_LIB(z, deflate,
	        [ZLIB="-lz" ZLTLIB=])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$zlib_libraries"
	      AC_CHECK_HEADER(zlib.h,
	       [AC_CHECK_LIB(z, deflate,
	        [ZLIB="-L$zlib_libraries -lz" ZLTLIB=])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_EXT_BZ2LIB,
 [BZ2INC="-I$srcdir/ext/bz2lib" BZ2LTLIB=libext-bz2lib.la BZ2LIB=
  AC_SUBST(BZ2INC)
  AC_SUBST(BZ2LIB)
  AC_SUBST(BZ2LTLIB)
  AC_ARG_WITH(bz2lib,
    [   --with-bz2lib                  Use separate libbz2],
    [ with_bz2lib_includes="${with_bz2lib_includes-$withval}"
      with_bz2lib_libraries="${with_bz2lib_libraries-$withval}" ])
  AC_ARG_WITH(bz2lib-includes,
    [   --with-bz2lib-includes=DIR     BZ2LIB headers are in DIR],
    [ bz2lib_includes="$withval"
      case "$bz2lib_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(bzlib.h,[BZ2INC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$bz2lib_includes"
	      AC_CHECK_HEADER(bzlib.h,[BZ2INC="-I$bz2lib_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(bz2lib-libraries,
    [   --with-bz2lib-libaries=DIR     BZ2LIB libraries are in DIR],
    [ bz2lib_libraries="$withval"
      case "$bz2lib_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(bzlib.h,
	       [AC_CHECK_LIB(bz2, BZ2_bzCompress,
	        [BZ2LIB="-lbz2" BZ2LTLIB=])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$bz2lib_libraries"
	      AC_CHECK_HEADER(bzlib.h,
	       [AC_CHECK_LIB(bz2, BZ2_bzCompress,
	        [BZ2LIB="-L$bz2lib_libraries -lbz2" BZ2LTLIB=])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_EXT_UUID,
 [UUIDINC="-Iext/uuid -I$srcdir/ext/uuid" UUIDLTLIB=libext-uuid.la UUIDLIB=
  AC_SUBST(UUIDINC)
  AC_SUBST(UUIDLIB)
  AC_SUBST(UUIDLTLIB)
  AC_ARG_WITH(uuid,
    [   --with-uuid                    Use separate libuuid],
    [ with_uuid_includes="${with_uuid_includes-$withval}"
      with_uuid_libraries="${with_uuid_libraries-$withval}" ])
  AC_ARG_WITH(uuid-includes,
    [   --with-uuid-includes=DIR       UUID headers are in DIR],
    [ uuid_includes="$withval"
      case "$uuid_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(uuid.h,[UUIDINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$uuid_includes"
	      AC_CHECK_HEADER(uuid.h,[UUIDINC="-I$uuid_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(uuid-libraries,
    [   --with-uuid-libaries=DIR       UUID libraries are in DIR],
    [ uuid_libraries="$withval"
      case "$uuid_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(uuid.h,
	       [AC_CHECK_LIB(uuid, uuid_clear,
	        [UUIDLIB="-luuid" UUIDLTLIB=])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$uuid_libraries"
	      AC_CHECK_HEADER(uuid.h,
	       [AC_CHECK_LIB(uuid, uuid_clear,
	        [UUIDLIB="-L$uuid_libraries -luuid" UUIDLTLIB=])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
  if test X"$UUIDLTLIB" != X; then
    AC_CONFIG_SUBDIRS([ext/uuid])
  fi
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_EXT_PCRE,
 [PCREINC="-Iext/pcre -I$srcdir/ext/pcre" PCRELTLIB=libext-pcre.la PCRELIB=
  AC_SUBST(PCREINC)
  AC_SUBST(PCRELIB)
  AC_SUBST(PCRELTLIB)
  AC_ARG_WITH(pcre,
    [   --with-pcre                    Use separate libpcre],
    [ with_pcre_includes="${with_pcre_includes-$withval}"
      with_pcre_libraries="${with_pcre_libraries-$withval}" ])
  AC_ARG_WITH(pcre-includes,
    [   --with-pcre-includes=DIR       PCRE headers are in DIR],
    [ pcre_includes="$withval"
      case "$pcre_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(pcre.h,[PCREINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$pcre_includes"
	      AC_CHECK_HEADER(pcre.h,[PCREINC="-I$pcre_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(pcre-libraries,
    [   --with-pcre-libaries=DIR       PCRE libraries are in DIR],
    [ pcre_libraries="$withval"
      case "$pcre_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(pcre.h,
	       [AC_CHECK_LIB(pcre, pcre_compile,
	        [PCRELIB="-lpcre" PCRELTLIB=])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$pcre_libraries"
	      AC_CHECK_HEADER(pcre.h,
	       [AC_CHECK_LIB(pcre, pcre_compile,
	        [PCRELIB="-L$pcre_libraries -lpcre" PCRELTLIB=])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
  if test X"$PCRELTLIB" != X; then
    AC_CONFIG_SUBDIRS([ext/pcre])
  fi
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_EXT_MD5,
 [MD5INC="-I$srcdir/ext/rfc1321" MD5LTLIB=libext-md5.la MD5LIB=
  AC_SUBST(MD5INC)
  AC_SUBST(MD5LIB)
  AC_SUBST(MD5LTLIB)
  AC_ARG_WITH(md5,
    [   --with-md5                     Use separate libmd5],
    [ with_md5_includes="${with_md5_includes-$withval}"
      with_md5_libraries="${with_md5_libraries-$withval}" ])
  AC_ARG_WITH(md5-includes,
    [   --with-md5-includes=DIR        MD5 headers are in DIR],
    [ md5_includes="$withval"
      case "$md5_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(md5.h,[MD5INC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$md5_includes"
	      AC_CHECK_HEADER(md5.h,[MD5INC="-I$md5_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(md5-libraries,
    [   --with-md5-libaries=DIR        MD5 libraries are in DIR],
    [ md5_libraries="$withval"
      case "$md5_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(md5.h,
	       [AC_CHECK_LIB(md5, MD5Init,
	        [MD5LIB="-lmd5" MD5LTLIB=])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$md5_libraries"
	      AC_CHECK_HEADER(md5.h,
	       [AC_CHECK_LIB(md5, MD5Init,
	        [MD5LIB="-L$md5_libraries -lmd5" MD5LTLIB=])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN(LAT_EXT_SHA1,
 [SHA1INC="-I$srcdir/ext/rfc3174" SHA1LTLIB=libext-sha1.la SHA1LIB=
  AC_SUBST(SHA1INC)
  AC_SUBST(SHA1LIB)
  AC_SUBST(SHA1LTLIB)
  AC_ARG_WITH(sha1,
    [   --with-sha1                    Use separate libsha1],
    [ with_sha1_includes="${with_md5_includes-$withval}"
      with_sha1_libraries="${with_md5_libraries-$withval}" ])
  AC_ARG_WITH(sha1-includes,
    [   --with-sha1-includes=DIR       SHA1 headers are in DIR],
    [ sha1_includes="$withval"
      case "$sha1_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(sha1.h,[SHA1INC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$sha1_includes"
	      AC_CHECK_HEADER(sha1.h,[SHA1INC="-I$sha1_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(sha1-libraries,
    [   --with-sha1-libaries=DIR       SHA1 libraries are in DIR],
    [ sha1_libraries="$withval"
      case "$sha1_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(sha1.h,
	       [AC_CHECK_LIB(sha1, SHA1Input,
	        [SHA1LIB="-lsha1" SHA1LTLIB=])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$sha1_libraries"
	      AC_CHECK_HEADER(sha1.h,
	       [AC_CHECK_LIB(sha1, SHA1Input,
	        [SHA1LIB="-L$sha1_libraries -lsha1" SHA1LTLIB=])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++#####################################################################
AC_DEFUN(LAT_EXT_AIXDL,
 [DLINC= DLLTLIB=
  AC_SUBST(DLINC)
  AC_SUBST(DLLTLIB)
  if test x$lat_cv_have_dlopen = xno &&
     test x$lat_cv_have_load = xyes &&
     test x$lat_cv_have_load_decl = xyes; then
    DLINC="-I$srcdir/ext/aix-dlfcn" DLLTLIB=libext-dl.la
  fi])
dnl ---######################################################################
dnl +++#####################################################################
AC_DEFUN(LAT_EXT_RX,
 [RXINC= RXLTLIB=
  AC_SUBST(RXINC)
  AC_SUBST(RXLTLIB)
  if test x$ac_cv_header_regex_h = xno; then
    # FIXME? rx doesn't seem to provide the header?
    mkdir -p ext/rx
    (echo '#ifdef __cplusplus'; echo 'extern "C" {'; echo '#endif';
     cat $srcdir/ext/rx/rxposix.h;
     echo '#ifdef __cplusplus'; echo '}'; echo '#endif';) > ext/rx/regex.h
    RXINC="-Iext/rx -I$srcdir/ext/rx" RXLTLIB=libext-rx.la
    AC_CONFIG_SUBDIRS([ext/rx])
  fi])
dnl ---######################################################################
