import FWCore.ParameterSet.Config as cms

VisSiPixelCluster = cms.EDAnalyzer('VisSiPixelCluster' ,
                                   moduleLabel = cms.untracked.string("siPixelClusters"),
                                   instanceName = cms.untracked.string(""),
                                   processName = cms.untracked.string("Rec")
                                   )
