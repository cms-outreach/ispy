import FWCore.ParameterSet.Config as cms

VisMET = cms.EDAnalyzer('VisMET' ,
                        moduleLabel = cms.untracked.string("htMetIC5"),
                        instanceName = cms.untracked.string(""),
                        processName = cms.untracked.string("Rec")
                        )
