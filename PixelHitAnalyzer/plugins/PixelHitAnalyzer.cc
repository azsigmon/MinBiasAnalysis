// -*- C++ -*-
//
// Package:    Analysis/PixelHitAnalyzer
// Class:      PixelHitAnalyzer
// 
/**\class PixelHitAnalyzer PixelHitAnalyzer.cc Analysis/PixelHitAnalyzer/plugins/PixelHitAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Anna Zsigmond
//         Created:  Wed, 18 Mar 2015 14:36:39 GMT
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

#include "TTree.h"

using namespace std;
using namespace edm;

static const int Max = 50000;
//
// class declaration
//

class PixelHitAnalyzer : public edm::EDAnalyzer {
   public:
      explicit PixelHitAnalyzer(const edm::ParameterSet&);
      ~PixelHitAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      edm::InputTag rechits_;

      TTree* Events;

      int evt, run, lumi;

      int mult;
      float x[Max];
      float y[Max];
      float z[Max];
      float phi[Max];
      float eta[Max];
      float clustersize[Max];

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
PixelHitAnalyzer::PixelHitAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   rechits_ = iConfig.getParameter<edm::InputTag> ("pixels");
}


PixelHitAnalyzer::~PixelHitAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
PixelHitAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   evt = iEvent.id().event();
   run = iEvent.id().run();
   lumi = iEvent.luminosityBlock();

   edm::ESHandle<TrackerGeometry> tGeo;
   iSetup.get<TrackerDigiGeometryRecord>().get(tGeo);

   Handle<SiPixelRecHitCollection> rchts;
   iEvent.getByLabel(rechits_,rchts);

   mult = 0;

   for (SiPixelRecHitCollection::const_iterator iter=rchts->begin(); iter!=rchts->end(); iter++) {
     for (edmNew::DetSet<SiPixelRecHit>::const_iterator hit=iter->begin(); hit!=iter->end(); hit++) {

       const SiPixelRecHit * recHit = &(*hit);

       const PixelGeomDetUnit* pixelLayer = dynamic_cast<const PixelGeomDetUnit*> (tGeo->idToDet(recHit->geographicalId()));
       GlobalPoint gpos = pixelLayer->toGlobal(recHit->localPosition());
       math::XYZVector rechitPos(gpos.x(),gpos.y(),gpos.z());

       x[mult] = gpos.x();
       y[mult] = gpos.y();
       z[mult] = gpos.z();
       eta[mult] = rechitPos.eta();
       phi[mult] = rechitPos.phi();
       clustersize[mult] = hit->cluster()->size();
       mult++;

     }
   }

   Events->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
PixelHitAnalyzer::beginJob()
{

   edm::Service<TFileService> fs;
   Events = fs->make<TTree>("Events", "");
   Events->Branch("evt",&evt,"evt/I");
   Events->Branch("run",&run,"run/I");
   Events->Branch("lumi",&lumi,"lumi/I");

   Events->Branch("mult",&mult,"mult/I");
   Events->Branch("x",x,"x[mult]/F");
   Events->Branch("y",y,"y[mult]/F");
   Events->Branch("z",z,"z[mult]/F");
   Events->Branch("eta",eta,"eta[mult]/F");
   Events->Branch("phi",phi,"phi[mult]/F");
   Events->Branch("clustersize",clustersize,"clustersize[mult]/F");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PixelHitAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PixelHitAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PixelHitAnalyzer);
