import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
process.load('RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_P_V53'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning2015/MinimumBias/RECO/PromptReco-v1/000/243/677/00000/4419DFB7-C2F8-E411-8128-02163E013444.root',
	'/store/data/Commissioning2015/MinimumBias/RECO/PromptReco-v1/000/243/677/00000/4E825EC1-BCF8-E411-B4E3-02163E0124F6.root',
        '/store/data/Commissioning2015/MinimumBias/RECO/PromptReco-v1/000/243/677/00000/F8104AB6-BCF8-E411-8F8C-02163E011CDC.root',
    )
)

process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string('analysisTree_Commissioning_v1__MinBias.root')
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

process.p = cms.Path(process.hltanalyzer
                    +process.toweranalyzer
		    #+process.siPixelRecHits*process.pixelhitanalyzer
                    #+process.trkanalyzer
)

