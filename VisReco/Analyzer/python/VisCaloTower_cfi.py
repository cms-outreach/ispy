import FWCore.ParameterSet.Config as cms

VisCaloTower = cms.EDAnalyzer('VisCaloTower' ,
                          moduleLabel = cms.untracked.string("towerMaker"),
                          instanceName = cms.untracked.string(""),
                          processName = cms.untracked.string("Rec")
                          )
