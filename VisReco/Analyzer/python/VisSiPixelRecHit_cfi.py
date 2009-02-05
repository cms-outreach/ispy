import FWCore.ParameterSet.Config as cms

VisSiPixelRecHit = cms.EDAnalyzer('VisSiPixelRecHit' ,
                                  moduleLabel = cms.untracked.string("siPixelRecHits"),
                                  instanceName = cms.untracked.string(""),
                                  processName = cms.untracked.string("Rec")
                                  )
