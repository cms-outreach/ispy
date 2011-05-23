cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

set(CXX c++)
set(CC gcc)
set(CPP cpp)
set(EXTERNALS_PREFIX sw)

ExternalProject_Add(zlib
                    URL http://zlib.net/zlib-1.2.5.tar.gz 
                    PREFIX ${EXTERNALS_PREFIX}
                    CONFIGURE_COMMAND cd <SOURCE_DIR> && ./configure --prefix <INSTALL_DIR> 
                    BUILD_COMMAND cd <SOURCE_DIR> && make -j 5
                    INSTALL_COMMAND cd <SOURCE_DIR> && make -j 10 install
)

ExternalProject_Add(freetype
                    DEPENDS zlib
                    URL http://ftp.igh.cnrs.fr/pub/nongnu/freetype/freetype-2.4.4.tar.gz
                    PREFIX ${EXTERNALS_PREFIX}
                    CONFIGURE_COMMAND cd <SOURCE_DIR> &&
                    ./configure --prefix <INSTALL_DIR>
                      ${PLATF_CFGFLAGS} --disable-libtool-lock 
                      --enable-static --disable-shared --with-zlib
                      CFLAGS='${PLATF_CFLAGS}' LDFLAGS='-L<INSTALL_DIR>/lib' LIBS='-lz'
                      CXX=${CXX} CC=${CC} CPP=${CPP}
                    BUILD_COMMAND cd <SOURCE_DIR> && make -j 5
                    INSTALL_COMMAND cd <SOURCE_DIR> && make -j 10 install
)


ExternalProject_Add(qt
                    DEPENDS zlib freetype
                    URL ftp://ftp.qt.nokia.com/qt/source/qt-everywhere-opensource-src-4.7.3.tar.gz
                    PREFIX ${EXTERNALS_PREFIX}
                    CONFIGURE_COMMAND cd <SOURCE_DIR> &&
                      yes | ./configure -prefix <INSTALL_DIR>
                        -static -fast -no-dwarf2 -release
                        -opensource -stl -L<INSTALL_DIR>/lib -no-glib
                        -no-libtiff -no-libpng -no-libmng
                        -qt-libjpeg -qt-libpng -qt-zlib
                        -no-separate-debug-info -no-multimedia
                        -no-sql-db2 -no-sql-ibase -no-sql-mysql 
                        -no-sql-oci -no-sql-odbc -no-sql-psql 
                        -no-sql-sqlite -no-sql-sqlite2 -no-sql-tds
                        -no-rpath -no-pch -no-dbus -no-xmlpatterns
                        -no-phonon -no-phonon-backend -no-svg -no-webkit
                        -declarative -no-qt3support -openssl-linked 
                        -make "libs tools"
                    BUILD_COMMAND cd <SOURCE_DIR> && make -j 4
                    INSTALL_COMMAND cd <SOURCE_DIR> && make -j 10 install
)

if (APPLE)
set(COIN_CONF_FLAGS --without-framework)
else(APPLE)
endif(APPLE)

ExternalProject_Add(coin
                    URL http://ftp.coin3d.org/coin/src/all/Coin-3.1.3.tar.gz 
                    PREFIX ${EXTERNALS_PREFIX}
                    CONFIGURE_COMMAND cd <SOURCE_DIR> &&
                      ./configure --prefix <INSTALL_DIR>
                        ${PLATF_CFGFLAGS} ${COIN_CONF_FLAGS} 
                        --disable-libtool-lock --disable-dependency-tracking
                        --enable-static --disable-shared --disable-dl-simage 
                        --disable-dl-openal --without-openal --without-zlib 
                        --disable-dl-zlib --disable-dl-libbzip2
                        --disable-java-wrapper --disable-3ds-import --disable-javascript-api
                        "CXXFLAGS=${USER_CXXFLAGS}"
                    BUILD_COMMAND cd <SOURCE_DIR> && make -j 5
                    INSTALL_COMMAND cd <SOURCE_DIR> && make -j 10 install
)


if(APPLE)
  set(PLATFORM APPLE)
else(APPLE)
  set(PLATFORM LINUX)
endif(APPLE)

set(CONFIG_QTLIBS -L<INSTALL_DIR>/lib -lz -lQt3Support -lQtOpenGL -lQtGui -lQtCore ${SOQT_LIBS})

ExternalProject_Add(soqt
                    URL http://ftp.coin3d.org/coin/src/all/SoQt-1.5.0.tar.gz
                    PREFIX ${EXTERNALS_PREFIX}
                    DEPENDS coin qt
                    CONFIGURE_COMMAND cd <SOURCE_DIR> && sh -ex ../../../configure_soqt.sh <INSTALL_DIR> ${PLATFORM}
                    BUILD_COMMAND cd <SOURCE_DIR> && make -j 5
                    INSTALL_COMMAND cd <SOURCE_DIR> && make -j 10 
)

if(APPLE)
set(SIMAGE_CONF_FLAGS --enable-quicktime --without-framework --without-x --enable-universal --enable-darwin-x11=false)
else(APPLE)
set(SIMAGE_CONF_FLAGS)
endif(APPLE)

ExternalProject_Add(simage
                    URL http://ftp.coin3d.org/coin/src/all/simage-1.7.0.tar.gz
                    PREFIX ${EXTERNALS_PREFIX}
                    DEPENDS qt
                    CONFIGURE_COMMAND cd <SOURCE_DIR> && 
                      QTDIR=<INSTALL_DIR> ./configure --disable-shared 
                        --prefix <INSTALL_DIR>
                        --enable-static --disable-libtool-lock --disable-dependency-tracking
                        --disable-debug --disable-symbols --without-targa --without-pic 
                        --without-rgb --without-tiff --without-jpeg --with-png 
                        --without-gif --without-ungif --without-oggvorbis
                        --without-eps --disable-framework
                    BUILD_COMMAND cd <SOURCE_DIR> ; make -j 5
                    INSTALL_COMMAND cd <SOURCE_DIR> ; make -j 10 install
)

ExternalProject_Add(libpng
                    URL ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.5.2.tar.gz 
                    PREFIX ${EXTERNALS_PREFIX}
                    CONFIGURE_COMMAND cd <SOURCE_DIR> &&
                      ./configure --prefix <INSTALL_DIR> --disable-dependency-tracking 
                                  --disable-shared
                    BUILD_COMMAND cd <SOURCE_DIR> ; make
                    INSTALL_COMMAND cd <SOURCE_DIR> ; make install
)


