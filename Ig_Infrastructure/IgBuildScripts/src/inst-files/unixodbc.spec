### RPM external unixodbc 2.2.6
Requires: zlib
Source: http://www.unixodbc.org/unixODBC-%v.tar.gz

%prep
%setup -n unixODBC-%v

%build
# FIXME: zlib, qt?, iconv?, libpng?
[ $(uname) = Darwin ] && export LDFLAGS=-L/sw/lib CPPFLAGS=-I/sw/include
./configure --prefix=%i --disable-gui --with-extra-libs=$ZLIB_ROOT/lib \
    --disable-dependency-tracking --disable-libtool-lock
    # --with-qt-dir=$QT_ROOT

# Somehow the libtool gets this wrong -- see README.OSX.  There's an
# "-all_load \$convenience", with the result that all objects are linked
# twice, producing many multiple symbol definitions.
[ $(uname) = Darwin ] &&
  perl -p -i -e 's/^whole_archive_flag_spec=.*/whole_archive_flag_spec=/' libtool
make

%install
make install
