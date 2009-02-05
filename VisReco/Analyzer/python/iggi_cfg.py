import FWCore.ParameterSet.Config as cms

process = cms.Process("IGUANA")
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v3/0000/F4323393-F3A3-DD11-B172-003048678EE2.root')
)
from FWCore.MessageLogger.MessageLogger_cfi import *

process.add_(
    cms.Service("IguanaService",
    outputFileName = cms.untracked.string('iggi.ig')
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(25)
)

process.load("VisReco.Analyzer.VisCaloTower_cfi")
process.load("VisReco.Analyzer.VisCSCSegment_cfi")
process.load("VisReco.Analyzer.VisDTRecSegment4D_cfi")
process.load("VisReco.Analyzer.VisJet_cfi")
process.load("VisReco.Analyzer.VisMET_cfi")
process.load("VisReco.Analyzer.VisPFCluster_cfi")
process.load("VisReco.Analyzer.VisPixelDigi_cfi")
process.load("VisReco.Analyzer.VisSiPixelCluster_cfi")
process.load("VisReco.Analyzer.VisSiPixelRecHit_cfi")
process.load("VisReco.Analyzer.VisSiStripCluster_cfi")
process.load("VisReco.Analyzer.VisSiStripDigi_cfi")
process.load("VisReco.Analyzer.VisTrack_cfi")

process.VisTrack.moduleLabel = "globalCosmicMuons"

process.p1 = cms.Path(process.VisTrack*
                      process.VisCSCSegment*
                      process.VisCaloTower*
                      process.VisDTRecSegment4D*
                      process.VisMET*
                      process.VisPFCluster*
                      process.VisPixelDigi*
                      process.VisSiPixelCluster*
                      process.VisSiPixelRecHit*
                      process.VisSiStripCluster*
                      process.VisSiStripDigi*
                      process.VisJet)

