#!/bin/sh

set -ex
rm -fr autom4te.cache
/usr/bin/glibtoolize --copy
/usr/bin/aclocal -I cfg
/usr/bin/automake --add-missing --copy
mv aclocal.m4 cfg
/usr/bin/autoconf -I cfg
perl -p -i -e 's,(\$\{wl\})?-(all|bind_at)_load,,' configure
perl -p -i -e 's,-install_name \$rpath/\$soname,-install_name \$soname,' configure
perl -p -i -e 's,(hardcode_libdir_flag_spec[_A-Za-z0-9]*)='\''.*,$1='\'\ \'',' configure
perl -p -i -e 's,(hardcode_libdir_separator[_A-Za-z0-9]*)='\''.*,$1=,' configure
rm -fr autom4te.cache
