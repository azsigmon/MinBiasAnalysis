// -*- C++ -*-
//
// Package:    Analysis/GenAnalyzer
// Class:      GenAnalyzer
// 
/**\class GenAnalyzer GenAnalyzer.cc Analysis/GenAnalyzer/plugins/GenAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Anna Zsigmond
//         Created:  Fri, 28 Nov 2014 09:08:02 GMT
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

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TTree.h"

using namespace std;

static const int Max = 50000;

//
// class declaration
//

class GenAnalyzer : public edm::EDAnalyzer {
   public:
      explicit GenAnalyzer(const edm::ParameterSet&);
      ~GenAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      edm::InputTag genparts_;

      TTree* Events;

      int evt, run, lumi;
      int nvtx;
      int processID;

      int mult;

      float energy[Max];
      float mass[Max];
      float pt[Max];
      float eta[Max];
      float phi[Max];
      float rapidity[Max];

      float vx[Max];
      float vy[Max];
      float vz[Max];

      int pdgid[Max];
      int charge[Max];
      int status[Max];

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
GenAnalyzer::GenAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   genparts_ = iConfig.getParameter<edm::InputTag> ("genparticles");
}


GenAnalyzer::~GenAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   evt = iEvent.id().event();
   run = iEvent.id().run();
   lumi = iEvent.luminosityBlock();

   edm::Handle<edm::View<PileupSummaryInfo>> pPUInfo;
   iEvent.getByLabel("addPileupInfo", pPUInfo);
   edm::View<PileupSummaryInfo>::const_iterator iterPU;
   nvtx = -1;
   for (iterPU = pPUInfo->begin(); iterPU != pPUInfo->end(); ++iterPU) // vector size is 3
   {
     int BX = iterPU->getBunchCrossing(); // -1: previous BX, 0: current BX,  1: next BX
     if (BX == 0) nvtx = iterPU->getPU_NumInteractions();
   }

   edm::Handle<GenEventInfoProduct> genevent;
   iEvent.getByLabel("generator", genevent);
   processID =  genevent->signalProcessID();

   edm::Handle<reco::GenParticleCollection> parts;
   iEvent.getByLabel(genparts_,parts);
   mult = 0;
   for(unsigned int i=0; i<parts->size(); i++){

     const reco::GenParticle& p = (*parts)[i];

     energy[mult] = p.energy();
     mass[mult] = p.mass();
     pt[mult] = p.pt();
     eta[mult] = p.eta();
     phi[mult] = p.phi();
     rapidity[mult] = p.rapidity();

     vx[mult] = p.px();
     vy[mult] = p.py();
     vz[mult] = p.pz();

     pdgid[mult] = p.pdgId();
     charge[mult] = p.charge();
     status[mult] = p.status();

     mult++;

   }

   Events->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
GenAnalyzer::beginJob()
{

   edm::Service<TFileService> fs;
   Events = fs->make<TTree>("Events", "");
   Events->Branch("evt",&evt,"evt/I");
   Events->Branch("run",&run,"run/I");
   Events->Branch("lumi",&lumi,"lumi/I");

   Events->Branch("nvtx",&nvtx,"nvtx/I");
   Events->Branch("processID",&processID,"processID/I");
   Events->Branch("mult",&mult,"mult/I");

   Events->Branch("energy",energy,"energy[mult]/F");
   Events->Branch("mass",mass,"mass[mult]/F");
   Events->Branch("pt",pt,"pt[mult]/F");
   Events->Branch("eta",eta,"eta[mult]/F");
   Events->Branch("phi",phi,"phi[mult]/F");
   Events->Branch("rapidity",rapidity,"rapidity[mult]/F");

   Events->Branch("vx",vx,"vx[mult]/F");
   Events->Branch("vy",vy,"vy[mult]/F");
   Events->Branch("vz",vz,"vz[mult]/F");

   Events->Branch("pdgid",pdgid,"pdgid[mult]/I");
   Events->Branch("charge",charge,"charge[mult]/I");
   Events->Branch("status",status,"status[mult]/I");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenAnalyzer);
