import FWCore.ParameterSet.Config as cms

VisPFCluster = cms.EDAnalyzer('VisPFCluster' ,
                              moduleLabel = cms.untracked.string(""),
                              instanceName = cms.untracked.string(""),
                              processName = cms.untracked.string("Rec")
                              )
