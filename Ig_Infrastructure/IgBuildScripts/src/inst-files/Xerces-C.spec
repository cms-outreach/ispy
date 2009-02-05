### RPM external Xerces-C 2.3.0
%define xercesv %(echo %v | tr . _)
Source: http://xml.apache.org/dist/xerces-c/archives/Xerces-C_%xercesv/xerces-c-src_%xercesv.tar.gz

%prep
%setup -n xerces-c-src_%xercesv

%build
export XERCESCROOT=$PWD
cd $PWD/src/xercesc
case $(uname) in
 Linux )
   ./runConfigure -P%i -plinux -cgcc -xg++ ;;
 Darwin )
   ./runConfigure -P%i -pmacosx -cgcc -xg++ ;;
esac
make

%install
export XERCESCROOT=$PWD
cd src/xercesc
make install
