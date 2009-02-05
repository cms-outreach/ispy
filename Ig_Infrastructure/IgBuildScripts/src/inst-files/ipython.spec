### RPM external ipython 0.6.0
Source: http://ipython.scipy.org/dist/IPython-%v.tar.gz
Requires: python

%prep
%setup -n IPython-%v

%build
%install
python setup.py install --prefix=%i
