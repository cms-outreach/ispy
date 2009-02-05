### RPM external qmtest 2.0.3
Requires: python
Source: http://www.codesourcery.com/public/qmtest/qm-%v/qm-%v.tar.gz

%prep
%setup -n qm-%v

%build
./configure --prefix=%i --with-python=$PYTHON_ROOT/bin/python
make

# %test
# make check
