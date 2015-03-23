import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

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
        'file:../../step3_RAW2DIGI_L1Reco_RECO.root'
    )
)

process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("test.root")
)

process.pixelhitanalyzer = cms.EDAnalyzer('PixelHitAnalyzer',
				     pixels = cms.InputTag("siPixelRecHits"),
)

process.p = cms.Path(process.siPixelRecHits*process.pixelhitanalyzer)

