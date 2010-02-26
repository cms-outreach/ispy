#!/bin/sh -xe

# Make sure that we are picking up the correct uname.
export PATH=$PREFIX/bin:$PATH
which qmake
ln -sf Iguana/QtGUI/scripts/ispy.pro ispy.pro
case `uname`:`uname -m` in
  Linux:x86_64 )
    ln -sf /usr/lib64/libSM.so.6.0.0 $PREFIX/lib/libSM.so
  ;;
  * )
  ;;
esac

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
    cp ./ispy ./ispy-debug
    strip ./ispy
    
    # Copy files twice as scp seems to fail once in a while, on first attempt.
    case `uname -m` in
      x86_64)
        scp ./ispy lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/linux/ispy-x86_64 || true
        scp ./ispy lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/linux/ispy-x86_64
        ;;
      *)
        scp ./ispy lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/linux/ispy || true
        scp ./ispy lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/linux/ispy
        scp ./ispy-debug lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/linux/ispy-debug || true
        scp ./ispy-debug lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/linux/ispy-debug
        ;;
    esac
    ;;
  Darwin)
    zip ispy.zip `find iSpy.app`
    scp ./ispy.zip lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/osx/ispy.zip || true
    scp ./ispy.zip lxplus.cern.ch:/afs/cern.ch/user/i/iguana/www/ispy-next/bin/osx/ispy.zip
    ;;
esac
