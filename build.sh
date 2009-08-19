#!/bin/sh
SCRAM_ARCH=slc4_ia32_gcc432
source /afs/cern.ch/cms/IGUANA/externals/cmsset_default.sh
scram p   IGUANA 200903191446-ge
mv Iguana 200903191446-ge/src
cd 200903191446-ge/src
scram b -v -j5 ispy
