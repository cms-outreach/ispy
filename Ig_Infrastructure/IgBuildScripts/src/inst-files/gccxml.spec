### RPM external gccxml 0.6.0

Requires: cmake
Source: http://www.gccxml.org/files/v0.6/%n-%v.tar.gz

%build
mkdir gccxml-build
cd gccxml-build
cmake -DCMAKE_INSTALL_PREFIX:PATH=%i ..
make

%install
cd gccxml-build
make install
