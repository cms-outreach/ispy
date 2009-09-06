#!/bin/sh -xe

# Make sure that we are picking up the correct uname.
export PATH=$PREFIX/bin:$PATH
which qmake
qmake
make -j 2

case `uname` in
  Linux)
    scp ./ispy lxplus.cern.ch:/afs/cern.ch/user/i/eulisse/www/ispy-next-`uname`-`uname m`
    ;;
  Darwin)
    zip ispy.zip `find iSpy.app`
    scp ./ispy lxplus.cern.ch:/afs/cern.ch/user/i/eulisse/www/ispy-next-`uname`
    ;;
esac
