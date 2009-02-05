### RPM external mysql 4.0.13
# 4.0.15?
# Source: ftp://mirror.mcs.anl.gov/pub/mysql/Downloads/MySQL-4.0/%n-%v.tar.gz
Source: http://downloads.mysql.com/archives/mysql-4.0/%n-%v.tar.gz

%prep
%setup -n %n-%v
%ifos darwin
# There's for some reason a "-traditional-cpp", which breaks with GCC 3.3
# so remove it.  (FIXME: check if this is solved in a newer version.)
perl -p -i -e 's/-traditional-cpp/-no-cpp-precomp/g' configure.in configure
%endif

%build
CFLAGS=-O3 CXX=gcc CXXFLAGS="-O3 -felide-constructors -fno-exceptions -fno-rtti" \
   ./configure --prefix=%i --with-extra-charsets=complex \
      --enable-thread-safe-client --enable-local-infile
make
