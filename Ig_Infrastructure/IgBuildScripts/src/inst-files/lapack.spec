### RPM external lapack 3.0.3
# Build system fudging by Lassi A. Tuura <lat@iki.fi>
# See also: http://www-jlc.kek.jp/~fujiik/macosx/10.2.X/SPECS/lapack.spec
Source0: http://www.netlib.org/lapack/lapack.tgz
Source1: http://www.netlib.org/lapack/manpages.tgz
Source2: http://www.netlib.org/lapack/lapackqref.ps
Source3: http://www.netlib.org/blas/blasqr.ps
Patch0: http://linuxfromscratch.mirror.ac.uk/sites/fedora.redhat.com/1/SRPMS/lapack-3.0-22.src.rpm/lapack-20010525.patch?extract=true
# Source4: Makefile.blas
# Source5: Makefile.lapack
# Patch0: lapack-20010525.patch

%prep
%setup -n LAPACK
%patch -p1

%build
cp INSTALL/make.inc.LINUX make.inc
make -C BLAS/SRC PLAT=
make -C SRC PLAT=

%install
mkdir -p %i/lib
cp -p blas.a %i/lib/libblas.a
cp -p lapack.a %i/lib/liblapack.a
ln -s liblapack.a %i/lib/liblapack3.a
ln -s libblas.a %i/lib/libblas3.a
