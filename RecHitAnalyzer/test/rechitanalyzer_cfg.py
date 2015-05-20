import FWCore.ParameterSet.Config as cms

process = cms.Process("HFRecHits")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'MCRUN2_74_V7'
process.GlobalTag.globaltag = 'GR_P_V54'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/afs/cern.ch/user/a/azsigmon/public/28F78240-9BFA-E411-86F2-0025905B8596.root',
        '/store/data/Commissioning2015/ZeroBias1/RECO/PromptReco-v1/000/243/677/00000/CC2188F0-B4F8-E411-AB54-02163E01185C.root'
    )
)

process.TFileService = cms.Service("TFileService",fileName=cms.string('test.root'))

process.rechitanalyzer = cms.EDAnalyzer('RecHitAnalyzer',
    recHits = cms.InputTag("hfreco")
)


process.p = cms.Path(process.rechitanalyzer)
