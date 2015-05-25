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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

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
      edm::InputTag vertices_;
      bool lightweight_;

      TTree* Events;

      int evt, run, lumi;

      int trkmult;
      float pt[Max];
      float eta[Max];
      float phi[Max];
      int charge[Max];
      float vx[Max];
      float vy[Max];
      float vz[Max];
      float normalizedChi2[Max];
      float dxy[Max];
      float dz[Max];
      float ptError[Max];
      float dxyError[Max];
      float dzError[Max];
      int numberOfValidHits[Max];
      int highPurity[Max];

      int vtxmult;
      float vtxX[Max];
      float vtxY[Max];
      float vtxZ[Max];

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
   vertices_ = iConfig.getParameter<edm::InputTag> ("vertices");
   lightweight_ = iConfig.getParameter<bool> ("lightweight");
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

   trkmult = 0;

   evt = iEvent.id().event();
   run = iEvent.id().run();
   lumi = iEvent.luminosityBlock();

   edm::Handle<vector<reco::Track>> trks;
   iEvent.getByLabel(tracks_,trks);

   for(unsigned int i=0; i<trks->size(); i++) {

     const reco::Track& p = (*trks)[i];

     pt[trkmult] = p.pt();
     eta[trkmult] = p.eta();
     phi[trkmult] = p.phi();
     charge[trkmult] = p.charge();
     vx[trkmult] = p.vx();
     vy[trkmult] = p.vy();
     vz[trkmult] = p.vz();
     normalizedChi2[trkmult] = p.normalizedChi2();
     dxy[trkmult] = p.dxy();
     dz[trkmult] = p.dz();
     ptError[trkmult] = p.ptError();
     dxyError[trkmult] = p.dxyError();
     dzError[trkmult] = p.dzError();
     numberOfValidHits[trkmult] = p.numberOfValidHits();
     highPurity[trkmult] = p.quality(reco::TrackBase::qualityByName("highPurity"));

     trkmult++;

   }

   vtxmult = 0;

   edm::Handle<reco::VertexCollection> recVertexCollection;
   iEvent.getByLabel(vertices_,recVertexCollection);
   const reco::VertexCollection * recVertices = recVertexCollection.product();

   for(reco::VertexCollection::const_iterator vertex = recVertices->begin(); vertex!= recVertices->end(); vertex++) {

     vtxX[vtxmult] = vertex->position().x();
     vtxY[vtxmult] = vertex->position().y();
     vtxZ[vtxmult] = vertex->position().z();

     vtxmult++;

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

   Events->Branch("trkmult",&trkmult,"trkmult/I");
   Events->Branch("pt",pt,"pt[trkmult]/F");
   Events->Branch("eta",eta,"eta[trkmult]/F");
   Events->Branch("phi",phi,"phi[trkmult]/F");
   Events->Branch("charge",charge,"charge[trkmult]/I");

   if(!lightweight_) {

   Events->Branch("vx",vx,"vx[trkmult]/F");
   Events->Branch("vy",vy,"vy[trkmult]/F");
   Events->Branch("vz",vz,"vz[trkmult]/F");

   Events->Branch("normalizedChi2",normalizedChi2,"normalizedChi2[trkmult]/F");
   Events->Branch("dxy",dxy,"dxy[trkmult]/F");
   Events->Branch("dz",dz,"dz[trkmult]/F");
   Events->Branch("ptError",ptError,"ptError[trkmult]/F");
   Events->Branch("dxyError",dxyError,"dxyError[trkmult]/F");
   Events->Branch("dzError",dzError,"dzError[trkmult]/F");
   Events->Branch("nValidHits",numberOfValidHits,"numberOfValidHits[trkmult]/I");
   Events->Branch("highPurity",highPurity,"highPurity[trkmult]/I");

   Events->Branch("vtxmult",&vtxmult,"vtxmult/I");
   Events->Branch("vtxX",vtxX,"vtxX[vtxmult]/F");
   Events->Branch("vtxY",vtxY,"vtxY[vtxmult]/F");
   Events->Branch("vtxZ",vtxZ,"vtxZ[vtxmult]/F");

   }

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
