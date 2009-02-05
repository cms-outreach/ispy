### RPM external cernlib 2003
## INITENV SET CERN $(dirname %i)
## INITENV SET CERN_ROOT %i
## INITENV SET CERN_LEVEL %v

# See also: http://hpc.sourceforge.net/
# See also: http://mommsen.home.cern.ch/mommsen/index.html?osx/cernlib.html
# See also: http://www-jlc.kek.jp/~fujiik/macosx/10.2.X/SPECS/lapack.spec
# Build system fudging by Lassi A. Tuura <lat@iki.fi>
# Minor tweaks by Lassi A. Tuura <lat@iki.fi>

%define cerndist http://wwwasd.web.cern.ch/wwwasd/cernlib/download/2003_source/tar
# define cerndist ftp://asisftp.cern.ch/asis/share/cern/%v/tar

Requires: libcompat lapack castor
Source0: %cerndist/src_car.tar.gz
Source1: %cerndist/src_cfortran.tar.gz
Source2: %cerndist/src_cmz.tar.gz
Source3: %cerndist/src_config.tar.gz
Source4: %cerndist/src_geant321.tar.gz
Source5: %cerndist/src_graflib.tar.gz
Source6: %cerndist/src_Imakefile.tar.gz
Source7: %cerndist/src_include.tar.gz
Source8: %cerndist/src_mathlib.tar.gz
Source9: %cerndist/src_mclibs.tar.gz
Source10: %cerndist/src_packlib.tar.gz
Source11: %cerndist/src_pawlib.tar.gz
Source12: %cerndist/src_phtools.tar.gz
Source13: %cerndist/src_scripts.tar.gz
Source14: %cerndist/src_xsneut95.tar.gz
Patch0: http://www-jlc.kek.jp/~fujiik/macosx/10.2.X/src/cernlib/patches/cernlib-%v-macosx.patch
Patch1: http://www-jlc.kek.jp/~fujiik/macosx/10.2.X/src/cernlib/patches/cernlib-%v-macosx.patch4
Patch2: cernlib-macosx-hacks
Patch3: cernlib-macosx-g77-link

%prep
if [ $(pwd | tr -dc '[A-Z]' | wc -c) != 0 -a \
     ! -d $(pwd | tr '[A-Z]' '[a-z]') ]; then
  (echo "The current directory $(pwd) contains capital letters and the"
   echo "file system is case-sensitive, and there is no lower-case-only"
   echo "alternative."
   echo
   echo "You are trying to build CERNLIB, which cannot be built in a"
   echo "directory with capital letters in it.  Please do the builds"
   echo "somewhere else where the entire path is accessible without"
   echo "capital letters."
  ) 1>&2
  exit 1
fi

%setup -T -n %v -b 0
%setup -D -T -n %v -b 1
%setup -D -T -n %v -b 2
%setup -D -T -n %v -b 3
%setup -D -T -n %v -b 4
%setup -D -T -n %v -b 5
%setup -D -T -n %v -b 6
%setup -D -T -n %v -b 7
%setup -D -T -n %v -b 8
%setup -D -T -n %v -b 9
%setup -D -T -n %v -b 10
%setup -D -T -n %v -b 11
%setup -D -T -n %v -b 12
%setup -D -T -n %v -b 13
%setup -D -T -n %v -b 14
%ifos darwin
%patch0
# The second patch has a broken MacOSX config part in the beginning, skip it
sed '1,/^diff/d' < %_sourcedir/cernlib-2003-macosx.patch4 | patch -p0
%patch2
%patch3
%endif
perl -p -i -e 's/ patchy / /' src/Imakefile

%build
mkdir -p %i/lib
rm -f %i/lib/liblapack* %i/lib/libblas* %i/lib/libcompat* %i/lib/libshift*
ln -s $LAPACK_ROOT/lib/liblapack* %i/lib
ln -s $LAPACK_ROOT/lib/libblas* %i/lib
ln -s $CASTOR_ROOT/lib/libshift* %i/lib
ln -s $LIBCOMPAT_ROOT/lib/libcompat* %i/lib

# Copy pristine source to install area
tar -cf - src | tar -C %i -xf -
mkdir -p %i/share
cp lib/*.dat %i/share

cd src
# FIXME: Does this work with !archfirst?
export CERN_LEVEL=%v
export CERN=$(dirname %i)
export CERN_ROOT=%i
export CVSCOSRC=$PWD
export PATH=$CERN_ROOT/bin:$PATH
mkdir -p $CERN_ROOT/bin $CERN_ROOT/lib

$CVSCOSRC/config/imake_boot
mkdir -p mgr
echo "tree all install install.lib install.bin includes::" > mgr/Makefile
make tree # make Makefiles # HAVE_MOTIF=yes?
make bin/kuipc
make -C scripts install.bin
make -j10 includes # (to generate deps)
make

rm -f %i/lib/*.dat
for f in */Makefile; do
  grep install.bin $f >/dev/null &&
    make -C $(dirname $f) install.bin
done
make DOCDIR=$CERN_ROOT/doc install install.lib
make CERN_INCLUDEDIR=$CERN_ROOT/include install.include

%install
# Do nothing, CERNLIB builds directly into the install area

# %test
# disable forever looping test in lepto63
# perl -p -i -e 's/\@.*PATHSEP.*demo/\@echo test skipped!/' mclibs/lepto63/test/Makefile
#
#  for d in packlib mathlib mclibs phtools geant321; do make -C $d test; done
#  make -C graflib/higz/examples higzex
#  (cd graflib/higz/examples; ./higzex)
#  (cd $CVSCOSRC/pawlib/paw/demo; paw all.kumac ) # go
