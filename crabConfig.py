from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")

config.General.requestName   = 'Ana_EPOS'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'runMC_cfg.py'

config.section_("Data")
config.Data.inputDataset = '/ReggeGribovPartonMC_13TeV-EPOS/azsigmon-MCRUN2_74_V7-v1-GEN-SIM-RAW-RECO-d15047e93fede7d97e3b31895963835c/USER'
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outLFN = '/store/group/phys_heavyions/azsigmon/pp2015/'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'

