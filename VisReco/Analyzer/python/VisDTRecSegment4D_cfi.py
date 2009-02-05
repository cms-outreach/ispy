import FWCore.ParameterSet.Config as cms

VisDTRecSegment4D = cms.EDAnalyzer('VisDTRecSegment4D' ,
                                   moduleLabel = cms.untracked.string("dt4DSegments"),
                                   instanceName = cms.untracked.string(""),
                                   processName = cms.untracked.string("Rec")
                                   )
