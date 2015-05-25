import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MCRUN2_74_V8'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring('root://xrootd.unl.edu//store/mc/RunIISpring15DR74/MinBias_TuneCUETP8M1_13TeV-pythia8/AODSIM/NoPU_MCRUN2_74_V8-v1/00000/04CD916F-33FA-E411-ACF0-0025902009E8.root'),
)

process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("tracktree.root"))

process.trkanalyzer = cms.EDAnalyzer('TrackAnalyzer',
				     tracks = cms.InputTag("generalTracks"),
				     vertices = cms.InputTag("offlinePrimaryVertices"),
				     lightweight = cms.bool(False),
)

process.p = cms.Path(process.trkanalyzer)
