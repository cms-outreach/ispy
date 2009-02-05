### RPM external mysql++ 1.7.9
# Local patches and build system fudging by Lassi A. Tuura <lat@iki.fi>
# FIXME: zlib usage?
Requires: mysql
Source: ftp://mirror.mcs.anl.gov/pub/mysql/Downloads/%n/%n-%v.tar.gz
Patch0: ftp://mirror.mcs.anl.gov/pub/mysql/Downloads/%n/%n-gcc-3.0.patch.gz
Patch1: ftp://mirror.mcs.anl.gov/pub/mysql/Downloads/%n/%n-gcc-3.2.patch.gz
Patch2: mysql++-strstream
Patch3: mysql++-connection
Patch4: mysql++-typeinfo

%prep
%setup -n %n-%v
%patch0 -p1
%patch1 -p1
%patch2
%patch3
%patch4
rm -f config.status
[ $(uname) = Darwin ] && cp /usr/share/libtool/config.* .

%build
./configure --prefix=%i \
    --with-mysql=$MYSQL_ROOT \
    --with-mysql-lib=$MYSQL_ROOT/lib/mysql \
    --with-mysql-include=$MYSQL_ROOT/include/mysql
perl -p -i -e 's/\@OBJEXT\@/o/g; s/\@EXEEXT\@//g' examples/Makefile
make
