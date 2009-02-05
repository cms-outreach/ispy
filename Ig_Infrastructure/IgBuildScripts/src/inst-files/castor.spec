### RPM external castor 1.7.1.4
# Patches for OS X by Lassi A. Tuura <lat@iki.fi> (FIXME: contribute?)
# Build system fudging by Lassi A. Tuura <lat@iki.fi>
%ifos darwin
# define fake -Dlinux -Dunix -D_IO_EOF_SEEN=__SEOF -D_IO_ERR_SEEN=__SERR -D_fileno=_file -DRLIMIT_AS=RLIMIT_DATA -Doff64_t=off_t -Dstat64=stat -Diovec64=iovec -Dlseek64=lseek -Dopen64=open -Dlstat64=lstat -Dfstat64=fstat -Dlockf64=lockf
# define fake -Dlinux -Dunix
# define fake -Dlinux -Dunix -D_IO_EOF_SEEN=__SEOF -D_IO_ERR_SEEN=__SERR -D_fileno=_file -DRLIMIT_AS=RLIMIT_DATA
%define fake -Dlinux -Dunix -D_IO_EOF_SEEN=__SEOF -D_IO_ERR_SEEN=__SERR -D_fileno=_file -DRLIMIT_AS=RLIMIT_DATA -Doff64_t=off_t
%endif

Source0: http://castor.web.cern.ch/castor/DIST/CERN/savannah/CASTOR.pkg/%v/CASTOR.tar.bz2
# Source1: http://cvs.opendarwin.org/index.cgi/~checkout~/Libcompat/4.4.subproj/cuserid.c?rev=1.1.1.1&cvsroot=apple
Patch0: shift
# Patch1: castor-osx

%prep
%setup -T -c -n castor -a 0
%patch0
%ifos darwin
# %patch1
# cp %_sourcedir/cuserid.c* common/cuserid.c
%endif

%build
# FIXME: Edit site.def?
(. ./setosflags
 imake -Dlinux -DUseInstalled -v -Iconfig \
   -DOSMajorVersion=$OSMAJNO -DOSMinorVersion=$OSMINNO Makefiles
 make Makefiles)
make -k RANLIB=ranlib CPPFLAGS="%{?fake:%fake}"

%install
make -k RANLIB=ranlib CPPFLAGS="%{?fake:%fake}" EXPORT=%i export
