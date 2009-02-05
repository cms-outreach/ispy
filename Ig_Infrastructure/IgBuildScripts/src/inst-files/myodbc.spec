### RPM external myodbc 3.51.06
Requires: mysql myodbc zlib
Source: ftp://mirror.mcs.anl.gov/pub/mysql/Downloads/MyODBC3/MyODBC-%v.tar.gz

%prep
%setup -n MyODBC-%v

%build
./configure --prefix=%i \
    --enable-thread-safe \
    --disable-libtool-lock \
    --disable-dependency-tracking \
    --with-unixODBC=$UNIXODBC_ROOT \
    --with-mysql-libs=$MYSQL_ROOT/lib/mysql \
    --with-mysql-includes=$MYSQL_ROOT/include/mysql \
    --with-ldflags=-L$ZLIB_ROOT/lib
make
