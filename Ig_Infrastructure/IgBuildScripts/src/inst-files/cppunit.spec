### RPM external cppunit 1.8.0
Source0: ftp://dl.sourceforge.net/pub/sourceforge/c/cp/%n/%n-%v.tar.gz
Source1: http://spi.cvs.cern.ch:8180/cgi-bin/spi.cgi/*checkout*/Components/UnitTesting/Tools/CppUnit/CppUnit_testdriver.cpp?rev=1.1

%install
make install
cp %_sourcedir/CppUnit_testdriver.cpp* %i/include/CppUnit_testdriver.cpp
