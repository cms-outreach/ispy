### RPM external coin 2.3.0
# Source: ftp://ftp.coin3d.org/pub/coin/src/snapshots/Coin-%v.tar.gz
# Source: cvs://:pserver:cvs@cvs.coin3d.org:2401/export/cvsroot?passwd=Ah<Z&tag=-D%{v}&module=Coin&output=/Coin-%v.tar.gz
Source: http://ftp.coin3d.org/coin/src/Coin-%v.tar.gz
Patch: coin

%prep
%setup -n Coin-%v
%patch

%build
cfgflags=; [ $(uname) = Darwin ] && cfgflags="-without-framework --without-x"
./configure --prefix=%i $cfgflags --disable-libtool-lock --disable-dependency-tracking
make
