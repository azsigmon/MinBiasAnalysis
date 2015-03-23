// -*- C++ -*-
//
// Package:    Analysis/TowerAnalyzer
// Class:      TowerAnalyzer
// 
/**\class TowerAnalyzer TowerAnalyzer.cc Analysis/TowerAnalyzer/plugins/TowerAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Anna Zsigmond
//         Created:  Tue, 17 Mar 2015 14:58:22 GMT
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

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

#include "TTree.h"

using namespace std;

static const int Max = 50000;
//
// class declaration
//

class TowerAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TowerAnalyzer(const edm::ParameterSet&);
      ~TowerAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      edm::InputTag towers_;

      TTree* Events;

      int evt, run, lumi;

      int mult;
      float energy[Max];
      float emEnergy[Max];
      float hadEnergy[Max];
      float outerEnergy[Max];
      float et[Max];
      float eta[Max];
      float phi[Max];
      int ieta[Max];
      int iphi[Max];

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
TowerAnalyzer::TowerAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   towers_ = iConfig.getParameter<edm::InputTag> ("calotowers");
}


TowerAnalyzer::~TowerAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TowerAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   evt = iEvent.id().event();
   run = iEvent.id().run();
   lumi = iEvent.luminosityBlock();

   edm::Handle<CaloTowerCollection> twrs;
   iEvent.getByLabel(towers_,twrs);
   mult = 0;

   for(unsigned int i=0; i<twrs->size(); i++) {

     const CaloTower &p= (*twrs)[i];

     energy[mult] = p.energy();
     emEnergy[mult] = p.emEnergy();
     hadEnergy[mult] = p.hadEnergy();
     outerEnergy[mult] = p.outerEnergy();
     et[mult] = p.et();
     eta[mult] = p.eta();
     phi[mult] = p.phi();
     ieta[mult] = p.ieta();
     iphi[mult] = p.iphi();

     mult++;

   }

   Events->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
TowerAnalyzer::beginJob()
{

   edm::Service<TFileService> fs;
   Events = fs->make<TTree>("Events", "");
   Events->Branch("evt",&evt,"evt/I");
   Events->Branch("run",&run,"run/I");
   Events->Branch("lumi",&lumi,"lumi/I");

   Events->Branch("mult",&mult,"mult/I");
   Events->Branch("energy",energy,"energy[mult]/F");
   Events->Branch("emEnergy",emEnergy,"emEnergy[mult]/F");
   Events->Branch("hadEnergy",hadEnergy,"hadEnergy[mult]/F");
   Events->Branch("outerEnergy",outerEnergy,"outerEnergy[mult]/F");
   Events->Branch("et",et,"et[mult]/F");
   Events->Branch("eta",eta,"eta[mult]/F");
   Events->Branch("phi",phi,"phi[mult]/F");
   Events->Branch("ieta",ieta,"ieta[mult]/I");
   Events->Branch("iphi",iphi,"iphi[mult]/I");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
TowerAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TowerAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TowerAnalyzer);
