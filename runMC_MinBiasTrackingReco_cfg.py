import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MCRUN2_74_V7::All'

###############################################################################
# Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    categories = cms.untracked.vstring(
      'MinBiasTracking', 'pixel3Vertices', 'NewVertices'
    ),
    debugModules = cms.untracked.vstring('*'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG'),
        DEBUG = cms.untracked.PSet(
            limit = cms.untracked.int32(1000000)
        )
    ),
    destinations = cms.untracked.vstring('cerr'),
    suppressError   = cms.untracked.vstring('globalPrimTracks','globalSecoTracks','globalTertTracks')
)

###############################################################################
# Input & output
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/afs/cern.ch/user/a/azsigmon/workspace/dNdeta/step3_RAW2DIGI_L1Reco_RECO.root'
    )
)

process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string('analysisTree_v3.root')
)

###############################################################################
# Agglomerative vertexing
import UserCode.FerencSiklerVertexing.NewVertexProducer_cfi
process.allVertices = UserCode.FerencSiklerVertexing.NewVertexProducer_cfi.newVertices.clone()
process.allVertices.TrackCollection = 'allTracks'

###############################################################################
# Paths
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

process.lreco = cms.Path(process.siPixelRecHits
                       * process.siStripMatchedRecHits)

# Minimum bias tracking and related
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
process.load("RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi")
process.load("RecoPixelVertexing.PixelLowPtUtilities.siPixelClusterShapeCache_cfi")
process.load("RecoPixelVertexing.PixelLowPtUtilities.MinBiasTracking_cff")

# Runge-Kutta
from TrackingTools.TrackFitters.RungeKuttaFitters_cff import *
KFFittingSmootherWithOutliersRejectionAndRK.EstimateCut = cms.double(50.)
KFFittingSmootherWithOutliersRejectionAndRK.LogPixelProbabilityCut = cms.double(-16.)

# Provide new shape files (all hits for pixels, only clear hits for strips)
from RecoPixelVertexing.PixelLowPtUtilities.ClusterShapeHitFilterESProducer_cfi import *
ClusterShapeHitFilterESProducer.PixelShapeFile = cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape_simHiteq1.par')
ClusterShapeHitFilterESProducer.StripShapeFile = cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/stripShape_simHiteq1.par')

process.greco = cms.Path(process.siPixelClusterShapeCache
                       * process.MeasurementTrackerEvent
                       * process.minBiasTracking
                       * process.allVertices)

###############################################################################
# Analyzers
process.genanalyzer = cms.EDAnalyzer('GenAnalyzer',
				     genparticles = cms.InputTag("genParticles"),
)

process.toweranalyzer = cms.EDAnalyzer('TowerAnalyzer',
				     calotowers = cms.InputTag("towerMaker"),
)

process.generaltrackana = cms.EDAnalyzer('TrackAnalyzer',
				     tracks = cms.InputTag("generalTracks"),
)

process.minbiastrackana = cms.EDAnalyzer('TrackAnalyzer',
				     tracks = cms.InputTag("allTracks"),
)

process.pixelhitanalyzer = cms.EDAnalyzer('PixelHitAnalyzer',
				     pixels = cms.InputTag("siPixelRecHits"),
)

process.postp = cms.Path(process.genanalyzer
                    	+process.toweranalyzer
                    	+process.generaltrackana
                    	+process.minbiastrackana
			+process.pixelhitanalyzer)

###############################################################################
# Schedule
process.schedule = cms.Schedule(process.lreco,
                                process.greco,
                                process.postp)

