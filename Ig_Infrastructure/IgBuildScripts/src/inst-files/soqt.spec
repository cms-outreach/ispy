### RPM external soqt 1.0.2
# 1.0.2-tgs, 1.0.2-coin (default: coin)
# FIXME: make type part of installation location!
%define soqttype %(echo %v | sed 's/[-0-9.]*//')
%define soqtversion %(echo %v | sed 's/-.*//')
%if "%soqttype" == ""
%define type	coin
%else
%define type    %{soqttype}
%endif
# define tgspath /some/path
# define qtpath  /some/path

Requires: qt
%if "%type" == "tgs"
Requires: tgsinventorfix
%else
Requires: coin
%endif

Source: ftp://ftp.coin3d.org/pub/coin/src/SoQt-%{soqtversion}.tar.gz
Patch: soqt

%prep
%setup -n SoQt-%{soqtversion}
%patch0

%build
case $(uname) in
  Darwin ) threads= cfgflags="-without-framework --without-x" ;;
  * )      threads=-pthread cfgflags= ;;
esac

# Allow Qt location to be overridden from RPM command line
%if "%{?qtpath:set}" == "set"
  QT_ROOT="%qtpath"
  export QT_ROOT
%endif

case "%type" in
  tgs )
    [ X"%tgspath" = X ] && { echo "%%tgspath not defined" 1>&2; exit 1; }

    # FIXME: OpenGL home?  (-L, --with-opengl=...)
    # FIXME: KCC/Linux needs -D__ELF__ -lz -lm
    # FIXME: KCC/Solaris needs -lm
    # FIXME: KCC/IRIX needs -lm; CC=$(CXX) CFLAGS=$(CFLAGS) (can't link C++ with KCC --c)
    # FIXME: Merge CFLAGS/CXXFLAGS to CC/CXX because of abi-changing meaning; zero out the flags
    # FIXME: Need -DENABLE_NLS -DEHAVE_GETTEXT for GCC 3.x on linuxen?
    # FIXME: Need --enable-exceptions?
    # FIXME: Need MAKE=...?
    # FIXME: Need -pthread / --enable-threads?
    ./configure --prefix=%i \
      --with-inventor=%tgspath --without-coin \
      --with-qt=$QT_ROOT --disable-dependency-tracking \
      --disable-libtool-lock $cfgflags \
      LDFLAGS="-L%tgspath" CXX="c++ $threads" \
      LIBS="-L$TGSINVENTORFIX_ROOT/lib -lTGSInventorFix" ;;

  coin )
    ./configure --prefix=%i --with-coin=$COIN_ROOT \
      --with-qt=$QT_ROOT --disable-dependency-tracking \
      --disable-libtool-lock $cfgflags \
      CXX="c++ $threads" ;;

  * )
    echo "unknown soqt build type %type" 1>&2
    exit 1 ;;
esac
make
