### RPM cms cms-tool-conf 90

Source0: none

Requires: gcc
Requires: AIDA
Requires: boost
Requires: bz2lib
Requires: clhep
Requires: edg-rls-client
Requires: gccxml
Requires: geant4
Requires: gsl
Requires: myodbc
Requires: mysql
Requires: mysql++
Requires: otl
Requires: pcre
Requires: python
Requires: root
Requires: uuid
Requires: unixodbc
Requires: Xerces-C
Requires: zlib
Requires: cppunit
Requires: oval
Requires: qmtest
Requires: valgrind
Requires: wxPython

Requires: SEAL
Requires: POOL
Requires: PI

Requires: qt
Requires: qutexmlrpc
Requires: coin
Requires: soqt
Requires: tgsinventorfix
Requires: cernlib
Requires: cmsim
Requires: castor
Requires: geant4

%prep
%build
(echo "ARCHITECTURE:%{cmsplatf}"
 echo "SCRAM_BASEPATH:%{instroot}/external"
 for f in %{pkgreqs}; do
   set -- $(echo $f | sed 's/\([^+]*\)+\(.*\)+\([0-9].*\)/\1 \2 \3/')
   case $2 in
     gcc )
       # FIXME: ccache, distcc?
%if "%{?use_system_gcc:set}" == "set"
       echo "TOOL:gcc3:"
       echo "  +GCC_BASE:/none"
       echo "  +CC:$(which gcc)"
       echo "  +CXX:$(which c++)"
       echo "  +PATH:/none"  # useless, toolbox says value=""
       echo "  +LD_LIBRARY_PATH:/none" # useless, toolbox says value=""
       echo "TOOL:g77gcc3:"
       echo "  +FC:$(which g77)"
%else
       echo "TOOL:gcc3:"
       echo "  +GCC_BASE:$GCC_ROOT"
       echo "TOOL:g77gcc3:"
       echo "  +FC:$GCC_ROOT/bin/g77"
%endif
       ;;

     geant4 )
       echo "TOOL:geant4:"
       echo "  +G4_BASE:$GEANT4_ROOT"
       echo "  +LIBDIR:$GEANT4_ROOT/lib"
       echo "  +INCLUDE:$GEANT4_ROOT/include"
       echo "  +G4SRC:$GEANT4_ROOT/source"
       ;;

     root )
       echo "TOOL:root:"
       echo "  +ROOT_BASE:$ROOT_ROOT"
       echo "  +LIBDIR:$ROOT_ROOT/lib"
       echo "  +INCLUDE:$ROOT_ROOT/include"
       echo "  +PATH:$ROOT_ROOT/bin"
       echo "  +ROOTSYS:$ROOT_ROOT"
       ;;

     python )
       echo "TOOL:python:"
       echo "  +PYTHON_BASE:$PYTHON_ROOT"
       echo "  +LIBDIR:$PYTHON_ROOT/lib"
       echo "  +INCLUDE:$PYTHON_ROOT/include/python2.2"
       echo "  +PATH:$PYTHON_ROOT/bin"
       ;;

     * )
       instunm=$(echo "$2" | tr '[a-z]' '[A-Z]' | sed 's/[^A-Z0-8]/_/g')
       lcgnm=$(echo "$2" | sed 's/+/p/g')
       lcgunm=$(echo "$lcgnm" | sed 's/-//g' | tr '[a-z]' '[A-Z]')
       lcglnm=$(echo "$lcgnm" | tr '[A-Z]' '[a-z]')
       eval known="\${${instunm}_ROOT+set}"
       if [ X$known = Xset ]; then
         echo "TOOL:$lcglnm:"
         eval "echo \"  +\${lcgunm}_BASE:\${${instunm}_ROOT}\""
         eval "echo \"  +PATH:\${${instunm}_ROOT}/bin\""
         eval "echo \"  +LIBDIR:\${${instunm}_ROOT}/lib\""
         eval "echo \"  +INCLUDE:\${${instunm}_ROOT}/include\""
       else
         echo "TOOL:$lcglnm:"
         eval "echo \"  +\${lcgunm}_BASE:/none\""
         eval "echo \"  +PATH:/none\""
         eval "echo \"  +LIBDIR:/none\""
         eval "echo \"  +INCLUDE:/none\""
       fi
       ;;
   esac
 done) > tool.conf

%install
cp tool.conf %i
