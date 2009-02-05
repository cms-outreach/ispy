### RPM external qutexmlrpc 0.1
# Patches by Lassi A. Tuura <lat@iki.fi> (FIXME: contribute)
Requires: qt
Source: ftp://dl.sourceforge.net/pub/sourceforge/q/qu/qutexr/%n-%v.tar.gz
Patch: qutexmlrpc

%prep
%setup -n %n

%build
# Allow Qt location to be overridden from RPM command line
%if "%{?qtpath:set}" == "set"
  QT_ROOT="%qtpath"
  export QT_ROOT
  unset QMAKESPEC || true
  export QTDIR=$QT_ROOT
  export PATH=$QTDIR/bin:$PATH
  export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
  export DYLD_LIBRARY_PATH=$QTDIR/lib:$DYLD_LIBRARY_PATH
%endif

cd src
qmake
make

%install
cd src
mkdir -p %i/lib %i/include/qutexmlrpc
tar -cf - libqutexr.* | tar -C %i/lib -xvf -
tar -cf - *.h | tar -C %i/include -xvf -
