#!qmake
#
# iSpy qmake project
# 

SUBDIRS += iSpyDCS
TEMPLATE = subdirs
CONFIG += release \
          warn_on \
          qt \
	  static \
          thread \
	  ordered

