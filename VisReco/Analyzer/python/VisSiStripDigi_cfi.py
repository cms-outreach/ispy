import FWCore.ParameterSet.Config as cms

VisSiStripDigi = cms.EDAnalyzer('VisSiStripDigi' ,
                                moduleLabel = cms.untracked.string("siStripDigis"),
                                instanceName = cms.untracked.string(""),
                                processName = cms.untracked.string("Rec")
                                )
