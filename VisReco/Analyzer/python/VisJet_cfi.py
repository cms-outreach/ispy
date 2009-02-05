import FWCore.ParameterSet.Config as cms

VisJet = cms.EDAnalyzer('VisJet' ,
                        moduleLabel = cms.untracked.string("iterativeCone5CaloJets"),
                        instanceName = cms.untracked.string(""),
                        processName = cms.untracked.string("Rec")
                        )
