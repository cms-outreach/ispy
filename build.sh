#!/bin/sh -xe

# Make sure that we are picking up the correct uname.
export PATH=$PREFIX/bin:$PATH
which qmake
qmake
make -j 2

scp ./bin/ispy lxplus.cern.ch:/afs/cern.ch/user/i/eulisse/www/ispy-next-`uname`
