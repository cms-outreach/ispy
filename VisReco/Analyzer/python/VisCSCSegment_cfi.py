import FWCore.ParameterSet.Config as cms

VisCSCSegment = cms.EDAnalyzer('VisCSCSegment' ,
                               moduleLabel = cms.untracked.string("cscSegments"),
                               instanceName = cms.untracked.string(""),
                               processName = cms.untracked.string("Rec")
                               )
