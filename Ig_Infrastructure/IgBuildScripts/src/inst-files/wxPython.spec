### RPM external wxPython 2.4.2.4
Source: ftp://dl.sourceforge.net/pub/sourceforge/w/wx/wxpython/%{n}Src-%v.tar.gz
Requires: python

%prep
%setup -n %{n}Src-%v

%build
WXPREF=%i
export WXPREF
mkdir build
cd build
../configure --prefix=$WXPREF \
             --enable-rpath=$WXPREF/lib \
             --with-opengl \
             --enable-geometry \
             --enable-optimise \
             --enable-debug_flag \
             --with-libjpeg=builtin \
             --with-libpng=builtin \
             --with-libtiff=builtin \
             --with-zlib=builtin
make -k
cd ../locale
make allmo

%install
WXPREF=%i
export WXPREF

cd build
make install
cd ../wxPython
python setup.py IN_CVS_TREE=1 WX_CONFIG=$WXPREF/bin/wx-config build install
