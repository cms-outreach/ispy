INSTALL_DIR=$1
PLATFORM=$2
case $PLATFORM in
  APPLE) 
    USER_CONF_FLAGS=--without-framework;;
  LINUX) ;;
esac
./configure --prefix $INSTALL_DIR \
     --disable-libtool-lock --disable-dependency-tracking \
     --enable-static --disable-shared --disable-dl-simage \
     --disable-dl-openal --without-openal --without-zlib \
     --disable-dl-zlib --disable-dl-libbzip2 \
     --disable-html \
     --with-freetype=$INSTALL_DIR --disable-dl-freetype \
     --with-fontconfig=$INSTALL_DIR --disable-dl-fontconfig \
     --disable-java-wrapper --disable-3ds-import --disable-javascript-api \
     $USER_CONF_FLAGS \
     LDFLAGS="-L$INSTALL_DIR/lib" \
     CPPFLAGS="-DFONTCONFIGGLUE_ASSUME_FONTCONFIG=1" \
     LIBS="-liconv -lfontconfig -lfreetype -lexpat -lz"
