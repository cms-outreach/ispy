### RPM external uuid 1.32
# Patches by Lassi A. Tuura <lat@iki.fi> (FIXME: contribute to e2fsprogs)
Source: ftp://dl.sourceforge.net/pub/sourceforge/e/e2/e2fsprogs/e2fsprogs-%v.tar.gz
Patch0: uuid
Patch1: uuid-osx

%prep
%setup -n e2fsprogs-%v
%patch0
%ifos darwin
%patch1
%endif

%build
./configure $([ $(uname) != Darwin ] && echo --enable-elf-shlibs) --prefix=%i
make lib/ext2fs/ext2_types.h
cd lib/uuid
make

%install
mkdir -p %i/lib
mkdir -p %i/include
cd lib/uuid
make install
