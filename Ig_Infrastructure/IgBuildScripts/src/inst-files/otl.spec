### RPM external otl 4.0.69
Source: http://otl.sourceforge.net/otlv4_h.zip

%prep
unzip %_sourcedir/*.zip

%build
%install
mkdir %i/include
cp *.h %i/include
