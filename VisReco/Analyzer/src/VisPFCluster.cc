#include "VisReco/Analyzer/interface/VisPFCluster.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFraction.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <qstring.h>
#include <iostream>
#include <sstream>

using namespace edm::service;

VisPFCluster::VisPFCluster(const edm::ParameterSet& iPSet)
    : friendlyName_(std::string()),
      moduleLabel_(std::string()),
      instanceName_(std::string()),
      processName_(std::string())

//: friendlyName_("recoPFClusters"),
//    moduleLabel_("particleFlowClusterECAL"),
//     instanceName_(""),
//     processName_("Rec")
{	
    friendlyName_ = iPSet.getUntrackedParameter<std::string>("friendlyName", std::string());
    moduleLabel_  = iPSet.getUntrackedParameter<std::string>("moduleLabel",  std::string());
    instanceName_ = iPSet.getUntrackedParameter<std::string>("instanceName", std::string());
    processName_  = iPSet.getUntrackedParameter<std::string>("processName",  std::string());
    
  
    if (friendlyName_.empty ()) 
    {
	edm::TypeID collID (typeid (reco::PFClusterCollection));
	friendlyName_ = collID.friendlyClassName ();
    }
}


void VisPFCluster::analyze(const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    
    if ( ! config.isAvailable() ) 
    {
	throw cms::Exception("Configuration")
	    << "VisPFCluster requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<reco::PFClusterCollection> collection;

    std::cout<< friendlyName_+" "+moduleLabel_+" "+instanceName_+" "+processName_<<std::endl;
    
    VisEventSelector visSel(friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get(visSel, collection);

    edm::ESHandle<CaloGeometry> caloGeometry;
    eventSetup.get<CaloGeometryRecord>().get(caloGeometry);

    if ( collection.isValid() && caloGeometry.isValid() )
    {	    
	IgDataStorage *storage = config->storage ();

	IgCollection& pfclusters = storage->getCollection("PFClusters_V1");
	
	IgProperty ID = pfclusters.addProperty("id", 0);
	IgProperty NAME = pfclusters.addProperty("name", std::string());
	
	IgCollection& rechits = storage->getCollection("RecHits_V1");
	
	IgProperty E = rechits.addProperty("energy", 0.0); 
	IgProperty FRAC = rechits.addProperty("fraction", 0.0);
	IgProperty LAYER = rechits.addProperty("layer", std::string());
	
	// or just make the corners an item of the rechit?
	IgCollection& corners = storage->getCollection("Corners_V1");

	IgProperty F1 = corners.addProperty("front1", IgV3d());
	IgProperty F2 = corners.addProperty("front2", IgV3d());
	IgProperty F3 = corners.addProperty("front3", IgV3d());
	IgProperty F4 = corners.addProperty("front4", IgV3d());
	
	IgProperty B1 = corners.addProperty("back1", IgV3d());
	IgProperty B2 = corners.addProperty("back2", IgV3d());
	IgProperty B3 = corners.addProperty("back3", IgV3d());
	IgProperty B4 = corners.addProperty("back4", IgV3d());
	
	IgAssociationSet& clusterRecHits = storage->getAssociationSet("PFClusterRecHits_V1");
	IgAssociationSet& recHitCorners = storage->getAssociationSet("RecHitCorners_V1");

	unsigned int i = 0;

	//std::cout<<"There are "<< collection->size() <<" PFClusters"<<std::endl;
	
	for ( std::vector<reco::PFCluster>::const_iterator cluster = collection->begin(), clusterEnd = collection->end();
	      cluster != clusterEnd; ++cluster )
	{
	    //std::cout<<"PFCluster #"<< i++ <<std::endl;

	    IgCollectionItem cl = pfclusters.create();
	    cl[ID] = i;
	    cl["name"] = std::string("PFCluster");
	    
	    std::vector<reco::PFRecHitFraction>::const_iterator iR;

	    //std::cout<<"There are "<< (*cluster).recHitFractions().size() <<" RecHits"<<std::endl;

	    for ( iR  = (*cluster).recHitFractions().begin();
		  iR != (*cluster).recHitFractions().end(); ++iR )
	    {	
		//std::cout<<"RecHit #"<< j++ <<std::endl;
		IgCollectionItem rh = rechits.create();
   
		double energy = (*iR).recHitRef()->energy();
		
		rh["energy"] = energy;
		rh["fraction"] = (*iR).fraction();		
		
		if ( (*cluster).layer() == PFLayer::ECAL_BARREL )
		    rh[LAYER] = std::string("EB");
			 
		else if ( (*cluster).layer() == PFLayer::ECAL_ENDCAP )
		    rh[LAYER] = std::string("EE");
		
		else
		    rh[LAYER] = std::string("UNDEFINED");
		  
		clusterRecHits.associate(cl,rh);
		
		// NOTE: Which corners are which depends on whether it's ECAL or HCAL.
		// Fix this later.

		double f1x, f1y, f1z;
		double f2x, f2y, f2z;
		double f3x, f3y, f3z;
		double f4x, f4y, f4z;
		
		double b1x, b1y, b1z;
		double b2x, b2y, b2z;
		double b3x, b3y, b3z;
		double b4x, b4y, b4z;
		
		const CaloCellGeometry::CornersVec& cornerPoints 
		    = (*caloGeometry).getGeometry((*iR).recHitRef()->detId())->getCorners();
		
		assert(cornerPoints.size() == 8);
		
		IgCollectionItem cr = corners.create();

		f1x = cornerPoints[3].x()/100.0;
		f1y = cornerPoints[3].y()/100.0;
		f1z = cornerPoints[3].z()/100.0;

		cr[F1] = IgV3d(f1x, f1y, f1z);
		
		f2x = cornerPoints[2].x()/100.0;
		f2y = cornerPoints[2].y()/100.0;
		f2z = cornerPoints[2].z()/100.0;

		cr[F2] = IgV3d(f2x, f2y, f2z);

		f3x = cornerPoints[1].x()/100.0;
		f3y = cornerPoints[1].y()/100.0;
		f3z = cornerPoints[1].z()/100.0;

		cr[F3] = IgV3d(f3x, f3y, f3z);

		f4x = cornerPoints[0].x()/100.0;
		f4y = cornerPoints[0].y()/100.0;
		f4z = cornerPoints[0].z()/100.0;

		cr[F4] = IgV3d(f4x, f4y, f4z);

		b1x = cornerPoints[7].x()/100.0;
		b1y = cornerPoints[7].y()/100.0;
		b1z = cornerPoints[7].z()/100.0;

		cr["back1"] = IgV3d(b1x, b1y, b1z);

		b2x = cornerPoints[6].x()/100.0;
		b2y = cornerPoints[6].y()/100.0;
		b2z = cornerPoints[6].z()/100.0;

		cr["back2"] = IgV3d(b2x, b2y, b2z);

		b3x = cornerPoints[5].x()/100.0;
		b3y = cornerPoints[5].y()/100.0;
		b3z = cornerPoints[5].z()/100.0;

		cr["back3"] = IgV3d(b3x, b3y, b3z);

		b4x = cornerPoints[4].x()/100.0;
		b4y = cornerPoints[4].y()/100.0;
		b4z = cornerPoints[4].z()/100.0;

		cr["back4"] = IgV3d(b4x, b4y, b4z);

		recHitCorners.associate(rh, cr);
		

		/*
		std::cout<<"F1: "<< f1x <<","<< f1y <<","<< f1z <<std::endl;
		std::cout<<"F2: "<< f2x <<","<< f2y <<","<< f2z <<std::endl;
		std::cout<<"F3: "<< f3x <<","<< f3y <<","<< f3z <<std::endl;
		std::cout<<"F4: "<< f4x <<","<< f4y <<","<< f4z <<std::endl;

		std::cout<<"B1: "<< b1x <<","<< b1y <<","<< b1z <<std::endl;
		std::cout<<"B2: "<< b2x <<","<< b2y <<","<< b2z <<std::endl;
		std::cout<<"B3: "<< b3x <<","<< b3y <<","<< b3z <<std::endl;
		std::cout<<"B4: "<< b4x <<","<< b4y <<","<< b4z <<std::endl;
		*/

	    } // Done iterating over RecHits

	    //std::cout<<"Done iterating over recHitFractions"<<std::endl;
	    
	} // Done iterating over PFClusters
    } // if collection is valid
    else 
    {
	std::cout<<"Collection not valid. No PFClusters?!"<<std::endl;
	
	std::string error = QString ("#Error: PFClusters with friendlyName %1 moduleLabel %2 "
				     "instanceName %3 processName %4 are not found.")
			    .arg (friendlyName_).arg (moduleLabel_).arg (instanceName_).arg (processName_).latin1 ();
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("Errors_V1");
	IgCollection &collection = storage->getCollection (text.c_str ());
 	IgProperty errorMsg = collection.addProperty ("Error", std::string ());
 	IgCollectionItem item = collection.create ();
 	item ["Error"] = error.c_str ();
    }
}

DEFINE_FWK_MODULE(VisPFCluster);
