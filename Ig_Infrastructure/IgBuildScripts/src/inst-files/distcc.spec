### RPM external distcc 2.10.1
Source: http://distcc.samba.org/ftp/%n/%n-%v.tar.bz2

%install
make install
ln -sf distcc %i/bin/cc
ln -sf distcc %i/bin/gcc
ln -sf distcc %i/bin/c++
ln -sf distcc %i/bin/g++
