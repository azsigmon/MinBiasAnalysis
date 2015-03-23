import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
process.load('RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MCRUN2_74_V7::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:step3_RAW2DIGI_L1Reco_RECO.root'
    )
)

process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string('analysisTree_v2.root')
)

process.genanalyzer = cms.EDAnalyzer('GenAnalyzer',
				     genparticles = cms.InputTag("genParticles"),
)

process.hltanalyzer = cms.EDAnalyzer('TriggerAnalyzer',
				     hltresults = cms.InputTag("TriggerResults","","HLT"),
)

process.toweranalyzer = cms.EDAnalyzer('TowerAnalyzer',
				     calotowers = cms.InputTag("towerMaker"),
)

process.trkanalyzer = cms.EDAnalyzer('TrackAnalyzer',
				     tracks = cms.InputTag("generalTracks"),
)

process.pixelhitanalyzer = cms.EDAnalyzer('PixelHitAnalyzer',
				     pixels = cms.InputTag("siPixelRecHits"),
)

process.p = cms.Path(process.genanalyzer
                    +process.hltanalyzer
                    +process.toweranalyzer
		    +process.siPixelRecHits*process.pixelhitanalyzer
                    +process.trkanalyzer)

