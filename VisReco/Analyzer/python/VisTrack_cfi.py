import FWCore.ParameterSet.Config as cms

VisTrack = cms.EDAnalyzer('VisTrack' ,
                          moduleLabel = cms.untracked.string("generalTracks"),
                          instanceName = cms.untracked.string(""),
                          processName = cms.untracked.string("Rec")
                          )
