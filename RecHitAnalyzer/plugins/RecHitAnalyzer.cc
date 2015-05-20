// -*- C++ -*-
//
// Package:    MinBiasAnalysis/RecHitAnalyzer
// Class:      RecHitAnalyzer
// 
/**\class RecHitAnalyzer RecHitAnalyzer.cc MinBiasAnalysis/RecHitAnalyzer/plugins/RecHitAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Oliver Suranyi
//         Created:  Wed, 20 May 2015 11:42:35 GMT
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

static const int Max = 50000;

//
// class declaration
//

class RecHitAnalyzer : public edm::EDAnalyzer {
   public:
      explicit RecHitAnalyzer(const edm::ParameterSet&);
      ~RecHitAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      // ----------member data ---------------------------
      
      edm::InputTag hfreco_;

      TTree* Events;

      int evt, run, lumi;

      int mult;
      int ieta[Max];
      int iphi[Max];
      float eta[Max];
      float phi[Max];
      float energy[Max];
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
RecHitAnalyzer::RecHitAnalyzer(const edm::ParameterSet& iConfig)
{
    hfreco_ = iConfig.getParameter<edm::InputTag> ("recHits");
}


RecHitAnalyzer::~RecHitAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
RecHitAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    evt = iEvent.id().event();
    run = iEvent.id().run();
    lumi = iEvent.luminosityBlock();

//    edm::SortedCollection<HFRecHit,edm::StrictWeakOrdering<HFRecHit> > hfrechits;
//    iEvent.getByLabel(hfreco,hfrechits);

    edm::Handle<edm::SortedCollection<HFRecHit,edm::StrictWeakOrdering<HFRecHit>>> coll;

    iEvent.getByLabel(hfreco_,coll);

    mult = 0;
    for(unsigned int i=0; i<coll->size(); i++){

        const HFRecHit& hit = (*coll)[i];
        
        ieta[i] = hit.id().ieta();
        iphi[i] = hit.id().iphi();
      //  eta[i] = hit.eta();
      //  phi[i] = hit.phi();
        energy[i] = hit.energy();
        mult++;
    }

//    int i = 0;
//    edm::SortedCollection<HFRecHit,edm::StrictWeakOrdering<HFRecHit>>::iterator coll_it;
//    for(auto it = coll_it.begin(); it != coll_it.end();it++){
//            eta[i] = it->id().ieta();
//            phi[i] = it->id().iphi();
//            i++;
//    }

    Events->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void 
RecHitAnalyzer::beginJob()
{
    edm::Service<TFileService> fs;
    Events = fs->make<TTree>("Events","");
   
    Events->Branch("evt",&evt,"evt/I");
    Events->Branch("run",&run,"run/I");
    Events->Branch("lumi",&lumi,"lumi/I");

    Events->Branch("mult",&mult,"mult/I");

    Events->Branch("ieta",ieta,"ieta[mult]/I");
    Events->Branch("iphi",iphi,"iphi[mult]/I");
    Events->Branch("eta",eta,"eta[mult]/F");
    Events->Branch("phi",phi,"phi[mult]/F");
    Events->Branch("energy",energy,"energy[mult]/F");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RecHitAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
RecHitAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
RecHitAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
RecHitAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
RecHitAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RecHitAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecHitAnalyzer);
