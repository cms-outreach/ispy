#!/bin/sh -xe

# Make sure that we are picking up the correct uname.
export PATH=$PREFIX/bin:$PATH
which qmake
ln -sf Iguana/QtGUI/scripts/ispy.pro ispy.pro

qmake
make -j 2

# In case there is some git repository information
# we use it to make up a label for the result.
# Otherwise the branch is always "next"
branchname=`git symbolic-ref HEAD | sed -e's|refs/heads/||' | tr / -`
if [ "X$branchname" = X ]
then
  branchname=next
fi



case `uname` in
  Linux)
    strip ./ispy
    scp ./ispy lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy/bin/linux/ispy-$branchname-`uname`-`uname -m`
    ;;
  Darwin)
    zip ispy.zip `find iSpy.app`
    scp ./ispy.zip lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy/bin/osx/ispy-$branchname.zip
    ;;
esac
