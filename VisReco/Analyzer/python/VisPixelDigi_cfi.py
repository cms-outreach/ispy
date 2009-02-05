import FWCore.ParameterSet.Config as cms

VisPixelDigi = cms.EDAnalyzer('VisPixelDigi' ,
                              moduleLabel = cms.untracked.string("siPixelDigis"),
                              instanceName = cms.untracked.string(""),
                              processName = cms.untracked.string("Rec")
                              )
