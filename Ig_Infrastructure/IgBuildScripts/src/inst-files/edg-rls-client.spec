### RPM external edg-rls-client 2.2.1
Source: http://service-spi.web.cern.ch/service-spi/external/distribution/edg-rls-client_%{v}__LCG_rh73_gcc32.tar.gz

%prep
[ $(uname) = Linux ] || { echo "%n is only available linux" 1>&2; exit 1; }
%setup -n %n/%v/rh73_gcc32

%build
%install
tar -cf - . | (cd %i; tar -xvvf -)
