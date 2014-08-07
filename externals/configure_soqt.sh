INSTALL_DIR=$1
PLATFORM=$2
case $PLATFORM in
  APPLE)
    SOQT_CXX=c++
    SOQT_LIBS="-framework AGL -framework Carbon -framework AppKit -framework ApplicationServices"
    SOQT_CONF_FLAGS="--without-framework --without-x --disable-darwin-x11"
  ;;  
  *)
    SOQT_CXX="c++ -pthread"
    SOQT_LIBS="-lrt -lfontconfig -lfreetype"
  ;;
esac

PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
CONFIG_QTLIBS="-L$INSTALL_DIR/lib -lz -lQtOpenGL -lQtGui -lQtCore $SOQT_LIBS"
./configure --prefix="$INSTALL_DIR" \
            --enable-static --disable-shared \
            --with-coin="$INSTALL_DIR" \
            --with-qt="$INSTALL_DIR" \
            --disable-dependency-tracking \
            --disable-libtool-lock \
            --disable-static-defaults \
            ${SOQT_CONF_FLAGS} \
            CXX="$SOQT_CXX" \
            LIBS="-L$INSTALL_DIR/lib $SOQT_LIBS" \
            QTDIR="$INSTALL_DIR" \
            CONFIG_QTLIBS="$CONFIG_QTLIBS"
make
make install
