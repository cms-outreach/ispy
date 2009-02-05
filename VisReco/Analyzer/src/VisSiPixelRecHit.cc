//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisSiPixelRecHit.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <qstring.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace edm::service;

VisSiPixelRecHit::VisSiPixelRecHit (const edm::ParameterSet& iPSet)
    : friendlyName_( std::string() ),
      moduleLabel_( std::string() ),
      instanceName_( std::string() ),
      processName_( std::string() )
{
    friendlyName_ = iPSet.getUntrackedParameter<std::string>( "friendlyName", std::string());
    moduleLabel_ = iPSet.getUntrackedParameter<std::string>( "moduleLabel", std::string());
    instanceName_ = iPSet.getUntrackedParameter<std::string>( "instanceName", std::string());
    processName_ = iPSet.getUntrackedParameter<std::string>( "processName", std::string());
    
    if (friendlyName_.empty ()) 
    {
	edm::TypeID collID (typeid (SiPixelRecHitCollection));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void 
VisSiPixelRecHit::analyze (const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisSiPixelRecHit requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<SiPixelRecHitCollection> collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<TrackerGeometry> geom;
    eventSetup.get<TrackerDigiGeometryRecord> ().get (geom);

    if (collection.isValid () && geom.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("SiPixelRecHits_V1");
	IgCollection &rechits = storage->getCollection (text.c_str ());

	IgProperty X = rechits.addProperty ("x", 0.0);
	IgProperty Y = rechits.addProperty ("y", 0.0);
	IgProperty Z = rechits.addProperty ("z", 0.0);

	SiPixelRecHitCollection::id_iterator it = collection->id_begin ();
	SiPixelRecHitCollection::id_iterator end = collection->id_end ();	

	for (; it != end; ++it)
	{
	    SiPixelRecHitCollection::range range = collection->get ((*it));
	    SiPixelRecHitCollection::const_iterator ipixel = range.first;
	    SiPixelRecHitCollection::const_iterator ipixelEnd = range.second;

	    float x = 0;
	    float y = 0;
	    float z = 0;
	    
	    for (; ipixel != ipixelEnd; ++ipixel)
	    {
		DetId detectorId = (*ipixel).geographicalId ();
		unsigned int id = detectorId.rawId ();
		  
		DetId detid (id);

		if((*ipixel).hasPositionAndError ())
		{		    
		    LocalPoint position = (*ipixel).localPosition ();
		    int clusterCharge = 0;

		    GlobalPoint pos = geom->idToDet (detid)->surface ().toGlobal (position);
		    x = pos.x () / 100.0;  // cm -> m
		    y = pos.y () / 100.0;  // cm -> m
		    z = pos.z () / 100.0;  // cm -> m

		    IgCollectionItem r = rechits.create ();
		    r ["x"] = static_cast<double>(pos.x ());
		    r ["y"] = static_cast<double>(pos.y ());
		    r ["z"] = static_cast<double>(pos.z ());
		}		
	    }
	}
    }
    else 
    {
	std::string error = QString ("### Error: SiPixel RecHits %1:%2:" // friendlyName:moduleLabel:instanceName:processName
				     "%3:%4 are not found.")
			    .arg (friendlyName_).arg (moduleLabel_).arg (instanceName_).arg (processName_).latin1 ();

	IgDataStorage *storage = config->storage ();
	std::string text = QString ("Errors_V1");
	IgCollection &collection = storage->getCollection (text.c_str ());
 	IgProperty errorMsg = collection.addProperty ("Error", std::string ());
 	IgCollectionItem item = collection.create ();
 	item ["Error"] = error.c_str ();
    }
}

DEFINE_FWK_MODULE(VisSiPixelRecHit);
