//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisSiPixelCluster.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
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

VisSiPixelCluster::VisSiPixelCluster (const edm::ParameterSet& iPSet)
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
	edm::TypeID collID (typeid (edmNew::DetSetVector<SiPixelCluster>));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void 
VisSiPixelCluster::analyze (const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisSiPixelCluster requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<edmNew::DetSetVector<SiPixelCluster> > collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<TrackerGeometry> geom;
    eventSetup.get<TrackerDigiGeometryRecord> ().get (geom);

    if (collection.isValid () && geom.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("SiPixelCluster_V1");
	IgCollection &clusters = storage->getCollection (text.c_str ());

	IgProperty X = clusters.addProperty ("x", 0.0);
	IgProperty Y = clusters.addProperty ("y", 0.0);
	IgProperty Z = clusters.addProperty ("z", 0.0);

	edmNew::DetSetVector<SiPixelCluster>::const_iterator it = collection->begin ();
	edmNew::DetSetVector<SiPixelCluster>::const_iterator end = collection->end ();

	for (; it != end; ++it)
	{
	    edmNew::DetSet<SiPixelCluster> ds = *it;
	    const uint32_t detID = it->detId ();
	    DetId detid (detID);

	    const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit *>(geom->idToDet (detid));
	    const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	    
	    edmNew::DetSet<SiPixelCluster>::const_iterator icluster = it->begin ();
	    edmNew::DetSet<SiPixelCluster>::const_iterator iclusterEnd = it->end ();

	    float x = 0;
	    float y = 0;
	    float z = 0;
	    
	    for(; icluster != iclusterEnd; ++icluster)
	    { 
		int row = (*icluster).minPixelRow ();
		int column =(*icluster).minPixelCol ();

		GlobalPoint pos =  (geom->idToDet (detid))->surface().toGlobal (theTopol->localPosition (MeasurementPoint (row, column)));
		x = pos.x () / 100.0;  // cm -> m;
		y = pos.y () / 100.0;  // cm -> m;
		z = pos.z () / 100.0;  // cm -> m;
		
		IgCollectionItem c = clusters.create ();
		c ["x"] = pos.x ();
		c ["y"] = pos.y ();
		c ["z"] = pos.z ();
	    }
	}
    }
    else 
    {
	std::string error = QString ("### Error: SiPixel Clusters %1:%2:" // friendlyName:moduleLabel:instanceName:processName
				     "%3:%4 are not found.")
			    .arg (friendlyName_).arg (moduleLabel_).arg (instanceName_).arg (processName_).latin1 ();

	IgDataStorage *storage = config->storage ();
	std::string text = QString ("Error_V1");
	IgCollection &collection = storage->getCollection (text.c_str ());
 	IgProperty errorMsg = collection.addProperty ("Error", std::string ());
 	IgCollectionItem item = collection.create ();
 	item ["Error"] = error.c_str ();
    }
}

DEFINE_FWK_MODULE(VisSiPixelCluster);
