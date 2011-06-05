INSTALL_DIR=$1
ARCH=$2
./configure --prefix $INSTALL_DIR --disable-libtool-lock \
                                   --enable-static --disable-shared \
                                   --with-expat=$INSTALL_DIR \
                                   --with-expat-lib=$INSTALL_DIR/lib \
                                   --with-expat-includes=$INSTALL_DIR/include \
                                   --with-freetype-config=$INSTALL_DIR/bin/freetype-config \
                                   LDFLAGS="-L$INSTALL_DIR/lib" \
                                   LIBS="-lz -lfreetype -lexpat-static"
