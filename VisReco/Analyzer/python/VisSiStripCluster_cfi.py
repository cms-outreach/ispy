import FWCore.ParameterSet.Config as cms

VisSiStripCluster = cms.EDAnalyzer('VisSiStripCluster' ,
                                   moduleLabel = cms.untracked.string("siStripClusters"),
                                   instanceName = cms.untracked.string(""),
                                   processName = cms.untracked.string("Rec")
                                   )
