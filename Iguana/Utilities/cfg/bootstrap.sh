#!/bin/sh

set -ex
rm -fr autom4te.cache
case $(uname) in
  Darwin)
    glibtoolize --copy --force ;;
  *)
    /usr/bin/libtoolize --copy --force ;;
esac
aclocal -I cfg
automake --add-missing --copy
mv aclocal.m4 cfg
autoconf -I cfg
perl -p -i -e 's,(\$\{wl\})?-(all|bind_at)_load,,g' configure cfg/ltmain.sh
perl -p -i -e 's,-install_name \$rpath/\$soname,-install_name \$soname,g' configure
perl -p -i -e 's,(hardcode_libdir_flag_spec[_A-Za-z0-9]*)='\''.*,$1='\'\ \'',' configure
perl -p -i -e 's,(hardcode_libdir_separator[_A-Za-z0-9]*)='\''.*,$1=,' configure
rm -fr autom4te.cache
