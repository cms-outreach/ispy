### RPM external cmsim 133
# Build system fudging can be blamed on Lassi A. Tuura <lat@iki.fi>
# Most of it was stolen from the CMSIM AFS area in /afs/cern.ch/cms/
# utils/{libbuild,config} (FIXME: download from ftp?)

Requires: cernlib
Source0: ftp://cmsdoc.cern.ch/cms/dist/cms%{v}_src.tar.gz
Source1: ftp://cmsdoc.cern.ch/cms/dist/cmsim_util_i386_linux24.tar.gz
Patch0: cmsim-patch

%prep
%setup -T -b 0 -n cmsim/cms%v/cmsim
%setup -D -T -a 1 -n cmsim/cms%v/cmsim
%patch0
export SRC=$PWD
mv utils/config config
perl -p -i -e "s|#define ConfigDir .*|#define ConfigDir $SRC/config|" config/site.def

%build
mkdir -p %i/lib

export SRC=$PWD
export CMS_PATH=%_builddir/cmsim
export CMS_VER=cms%v

# Build CMSIM libraries
DFLAGS="-DMYSRC=$SRC -DLIBSDIR=%i/lib -Dlinux"
imake -I$SRC/config $DFLAGS -DDFLAGS="$DFLAGS"
make Makefiles
make all
ranlib %i/lib/lib*.a

exit 0

#$$$$ NB: the rest has been disabled, binary is not needed any more
# Build CMSIM executable
makefile="%_builddir/cmsim/cms%v/examples/cmsim/GNUmakefile"
vars=
vars="$vars 'BUILDDIR=%_builddir'"
vars="$vars 'CMS_LIBS=%i/lib'"
vars="$vars 'CMSUSER_LIB='"
vars="$vars 'CMSUSER_LIBDIR='"
vars="$vars 'DefaultMakefile=$makefile'"
# vars="$vars interactive=i"
# vars="$vars debug=_d"
# vars="$vars CMSMotif=true"
# vars="$vars module_foo=1"
# vars="$vars SCRATCH=..."
# cmsim cms${version}
if [ $(uname) = Darwin ]; then
  vars="$vars 'UNAME=Linux'"
  vars="$vars 'FC=g77'"
  vars="$vars 'SHRLIB=libecpp.dylib'"
  vars="$vars 'DOSHARED=c++ -o \$(SHRLIB) -dynamiclib -single_module'"
  vars="$vars 'CXXFLAGS=-fPIC'"
fi
eval make -f $makefile clean_switch $vars
eval make -f $makefile $vars

%install
exit 0
#$$$ NB: disabled, binary not needed any more
# mkdir -p %i/bin
# cp %_builddir/cms133.exe %i/bin
