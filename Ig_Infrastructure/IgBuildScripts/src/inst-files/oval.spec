### RPM external oval 3_5_0
Source: http://polywww.in2p3.fr/cms/software/oval/%{n}_%{v}.tar.gz

%prep
%setup -n %v

%build
%install
tar -cf - . | tar -C %i -xf -
find %i -type d -name CVS -print | xargs rm -fr
