### RPM external ccache 2.3
## INITENV SET CCACHE_DIR %buildroot/.ccache
## INITENV CMD ccache -M 1G
Source: http://ccache.samba.org/ftp/%n/%n-%v.tar.gz

%install
make install
ln -sf ccache %i/bin/cc
ln -sf ccache %i/bin/gcc
ln -sf ccache %i/bin/c++
ln -sf ccache %i/bin/g++
