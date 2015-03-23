// -*- C++ -*-
//
// Package:    Analysis/TriggerAnalyzer
// Class:      TriggerAnalyzer
// 
/**\class TriggerAnalyzer TriggerAnalyzer.cc Analysis/TriggerAnalyzer/plugins/TriggerAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Anna Zsigmond
//         Created:  Thu, 27 Nov 2014 14:28:31 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "TTree.h"

using namespace std;

const static int MAXP = 10000;

//
// class declaration
//

class TriggerAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TriggerAnalyzer(const edm::ParameterSet&);
      ~TriggerAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      edm::InputTag hltresults_;

      TTree* Events;

      int evt, run, lumi;
      bool firstEvent;
      int* trigflag;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TriggerAnalyzer::TriggerAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   hltresults_ = iConfig.getParameter<edm::InputTag> ("hltresults");

}


TriggerAnalyzer::~TriggerAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TriggerAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   evt = iEvent.id().event();
   run = iEvent.id().run();
   lumi = iEvent.luminosityBlock();

   edm::Handle<edm::TriggerResults>  hltresults;
   iEvent.getByLabel(hltresults_,hltresults);
   edm::TriggerNames const& triggerNames = iEvent.triggerNames(*hltresults);

   int ntrigs = hltresults->size();

   if (firstEvent){
     for (int itrig = 0; itrig != ntrigs; ++itrig) {
       TString trigName = triggerNames.triggerName(itrig);
       Events->Branch(trigName, trigflag+itrig, trigName+"/I");
     }
     firstEvent = false;
   }

   for (int itrig = 0; itrig != ntrigs; ++itrig){

     bool accept = hltresults->accept(itrig);
     if (accept){trigflag[itrig] = 1;}
     else {trigflag[itrig] = 0;}

   }

   Events->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
TriggerAnalyzer::beginJob()
{

   edm::Service<TFileService> fs;
   Events = fs->make<TTree>("Events", "");
   Events->Branch("evt",&evt,"evt/I");
   Events->Branch("run",&run,"run/I");
   Events->Branch("lumi",&lumi,"lumi/I");

   firstEvent = true;
   const int Max = 100;
   trigflag = new int[Max];

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TriggerAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerAnalyzer);
