#!/bin/sh -xe

# Make sure that we are picking up the correct uname.
export PATH=$PREFIX/bin:$PATH
which qmake
ln -sf Iguana/QtGUI/scripts/ispy.pro ispy.pro

qmake
make -j 2

case `uname` in
  Linux)
    scp ./ispy eulisse@lxplus.cern.ch:/afs/cern.ch/user/i/eulisse/www/ispy-next-`uname`-`uname m`
    ;;
  Darwin)
    zip ispy.zip `find iSpy.app`
    scp ./ispy.zip eulisse@lxplus.cern.ch:/afs/cern.ch/user/i/eulisse/www/ispy-next-`uname`
    ;;
esac
