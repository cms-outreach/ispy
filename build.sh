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
branchname=next
if [ -d .git ]
then
  branchname=`git symbolic-ref HEAD | sed -e's|refs/heads/||' | tr / -`
fi



case `uname` in
  Linux)
    scp ./ispy eulisse@lxplus.cern.ch:/afs/cern.ch/user/i/eulisse/www/ispy-next-`uname`-`uname -m`
    ;;
  Darwin)
    zip ispy.zip `find iSpy.app`
    scp ./ispy.zip eulisse@lxplus.cern.ch:/afs/cern.ch/user/i/eulisse/www/ispy-next-`uname`
    ;;
esac
