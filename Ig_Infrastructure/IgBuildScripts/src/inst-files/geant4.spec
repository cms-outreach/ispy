### RPM external geant4 5.2.p01
# Build system fudging and some patches by Lassi A. Tuura <lat@iki.fi>
Requires: clhep
Source0: http://wwwasd.web.cern.ch/wwwasd/geant4/source/source/geant4.%v.tar.gz
Source1: http://wwwasd.web.cern.ch/wwwasd/geant4/source/source/G4NDL3.7.tar.gz
Source2: http://wwwasd.web.cern.ch/wwwasd/geant4/source/source/G4EMLOW2.2.tar.gz
Source3: http://wwwasd.web.cern.ch/wwwasd/geant4/source/source/PhotonEvaporation.2.0.tar.gz
Source4: http://wwwasd.web.cern.ch/wwwasd/geant4/source/source/RadiativeDecay.3.0.tar.gz
Patch0: http://www-jlc.kek.jp/~hoshina/cdc/geant4/sources/patch/MacOSX/Geant4.5.0-macosx10.2gcc3.patch
Patch1: geant4-macosx-2

%prep
%setup -n %n.%v
%ifos darwin
#patch0 -p1
%patch1
%endif

%build
# Linux? -pthread?
export G4SYSTEM=$(uname)-g++
export G4INSTALL=%i
export G4BASE=$PWD/source
export G4WORKDIR=$PWD
export G4TMP=$PWD/tmp

export G4LIB_BUILD_SHARED=1
export G4DEBUG=1
# export G4LIB_BUILD_STATIC=1
# FIXME: For OS X? export G4NO_OPTIMISE=1 // unset G4OPTIMISE
# FIXME: override CERNLIB_PATH?

export CLHEP_BASE_DIR=$CLHEP_ROOT

export G4USE_STL=1
# export G4USE_G3TOG4=1

export G4UI_BUILD_TERMINAL_SESSION=1
# export G4UI_BUILD_GAG_SESSION=1
# export G4UI_BUILD_XAW_SESSION=1
# export G4UI_BUILD_XM_SESSION=1
# export G4UI_BUILD_WO_SESSION=1

export OGLHOME=/usr/X11R6
# export OGLLIBS="-L$OGLHOME/lib -lGLU -lGL"
# export OGLFLAGS="-I$OGLHOME/include"

export G4VIS_BUILD_DAWNFILE_DRIVER=1
# export G4VIS_BUILD_DAWN_DRIVER=1
# export G4VIS_BUILD_OPENGLX_DRIVER=1
# export G4VIS_BUILD_OPENGLXM_DRIVER=1
export G4VIS_BUILD_VRMLFILE_DRIVER=1
export G4VIS_BUILD_VRML_DRIVER=1
export G4VIS_BUILD_RAYTRACER_DRIVER=1

mkdir -p %i
tar -cf - config source | tar -C %i -xf -
make -C $G4BASE all
make -C $G4BASE includes

%install
# Build already installed into prefix
mkdir -p %i/data
tar -C %i/data -zxvf %_sourcedir/G4NDL*.tar.gz
tar -C %i/data -zxvf %_sourcedir/G4EMLOW*.tar.gz
tar -C %i/data -zxvf %_sourcedir/Photon*.tar.gz
tar -C %i/data -zxvf %_sourcedir/Rad*.tar.gz
