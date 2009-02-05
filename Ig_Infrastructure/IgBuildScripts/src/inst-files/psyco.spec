### RPM external psyco 1.2
Source: ftp://dl.sourceforge.net/pub/sourceforge/p/ps/%n/%n-%v-src.tar.gz
Requires: python

%prep
%setup -n %n-%v

%build
%install
python setup.py install --prefix=%i
