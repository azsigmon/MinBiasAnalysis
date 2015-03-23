// -*- C++ -*-
//
// Package:    Analysis/TrackAnalyzer
// Class:      TrackAnalyzer
// 
/**\class TrackAnalyzer TrackAnalyzer.cc Analysis/TrackAnalyzer/plugins/TrackAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Anna Zsigmond
//         Created:  Thu, 05 Feb 2015 08:51:47 GMT
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

#include "DataFormats/TrackReco/interface/Track.h"

#include "TTree.h"

using namespace std;

static const int Max = 50000;
//
// class declaration
//

class TrackAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TrackAnalyzer(const edm::ParameterSet&);
      ~TrackAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      edm::InputTag tracks_;

      TTree* Events;

      int evt, run, lumi;

      int mult;
      float pt[Max];
      float eta[Max];
      float phi[Max];
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
TrackAnalyzer::TrackAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   tracks_ = iConfig.getParameter<edm::InputTag> ("tracks");
}


TrackAnalyzer::~TrackAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   mult = 0;

   evt = iEvent.id().event();
   run = iEvent.id().run();
   lumi = iEvent.luminosityBlock();

   edm::Handle<vector<reco::Track>> trks;
   iEvent.getByLabel(tracks_,trks);

   for(unsigned int i=0; i<trks->size(); i++){

     const reco::Track& p = (*trks)[i];

     pt[mult] = p.pt();
     eta[mult] = p.eta();
     phi[mult] = p.phi();

     mult++;

   }

   Events->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
TrackAnalyzer::beginJob()
{

   edm::Service<TFileService> fs;
   Events = fs->make<TTree>("Events", "");
   Events->Branch("evt",&evt,"evt/I");
   Events->Branch("run",&run,"run/I");
   Events->Branch("lumi",&lumi,"lumi/I");

   Events->Branch("mult",&mult,"mult/I");
   Events->Branch("pt",pt,"pt[mult]/F");
   Events->Branch("eta",eta,"eta[mult]/F");
   Events->Branch("phi",phi,"phi[mult]/F");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrackAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TrackAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackAnalyzer);
