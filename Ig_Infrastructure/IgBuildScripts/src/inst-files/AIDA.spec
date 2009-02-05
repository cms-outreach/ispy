### RPM external AIDA 3.0.0
Source: ftp://ftp.slac.stanford.edu/software/freehep/AIDA/v%v/aida-%v-src.tar.gz

%prep
%setup -n src/cpp

%build

%install
mkdir -p %i/include
tar -cf - AIDA | tar -C %i/include -xf -
