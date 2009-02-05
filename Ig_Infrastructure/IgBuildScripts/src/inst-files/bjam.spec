### RPM external bjam 3.1.4
Source: ftp://dl.sourceforge.net/pub/sourceforge/b/bo/boost/boost-jam-%v.tgz

%prep
%setup -n boost-jam-%v

%build
case $(uname) in
  Darwin ) sh build.sh darwin ;;
  * )      sh build.sh gcc ;;
esac

%install
mkdir -p %i
cp -r bin.* %i/bin
