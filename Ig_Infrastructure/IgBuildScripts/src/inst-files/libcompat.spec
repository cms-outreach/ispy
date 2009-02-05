### RPM external libcompat 1.14
Source: http://cvs.opendarwin.org/index.cgi/~checkout~/Libcompat/4.4.subproj/cuserid.c?rev=1.1.1.1&cvsroot=apple

%prep
cp %_sourcedir/cuserid.c* cuserid.c

%build
gcc -c cuserid.c
ar cr libcompat.a cuserid.o
ranlib libcompat.a

%install
mkdir -p %i/lib
cp libcompat.a %i/lib
