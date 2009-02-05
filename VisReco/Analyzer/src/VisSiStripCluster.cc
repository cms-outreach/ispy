//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisSiStripCluster.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
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

VisSiStripCluster::VisSiStripCluster (const edm::ParameterSet& iPSet)
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
	edm::TypeID collID (typeid (edmNew::DetSetVector<SiStripCluster>));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void 
VisSiStripCluster::analyze (const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisSiStripCluster requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<edmNew::DetSetVector<SiStripCluster> > collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<TrackerGeometry> geom;
    eventSetup.get<TrackerDigiGeometryRecord> ().get (geom);

    if (collection.isValid () && geom.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("SiStripClusters_V1");
	IgCollection &clusters = storage->getCollection (text.c_str ());

	IgProperty X = clusters.addProperty ("x", 0.0);
	IgProperty Y = clusters.addProperty ("y", 0.0);
	IgProperty Z = clusters.addProperty ("z", 0.0);

	edmNew::DetSetVector<SiStripCluster>::const_iterator it = collection->begin ();
	edmNew::DetSetVector<SiStripCluster>::const_iterator end = collection->end ();

	for (; it != end; ++it)
	{
	    edmNew::DetSet<SiStripCluster> ds = *it;
	    const uint32_t detID = it->detId ();
	    DetId detid (detID);

	    const StripGeomDetUnit* theDet = dynamic_cast<const StripGeomDetUnit *>(geom->idToDet (detid));
	    const StripTopology* theTopol = dynamic_cast<const StripTopology *>( &(theDet->specificTopology ()));

	    edmNew::DetSet<SiStripCluster>::const_iterator icluster = it->begin ();
	    edmNew::DetSet<SiStripCluster>::const_iterator iclusterEnd = it->end ();

	    float x = 0;
	    float y = 0;
	    float z = 0;
	    
	    for(; icluster != iclusterEnd; ++icluster)
	    { 
		short firststrip = (*icluster).firstStrip ();
		GlobalPoint pos =  (geom->idToDet (detid))->surface().toGlobal (theTopol->localPosition (firststrip));
		IgCollectionItem c = clusters.create ();
		c ["x"] = static_cast<double>(pos.x () / 100.0);
		c ["y"] = static_cast<double>(pos.y () / 100.0);
		c ["z"] = static_cast<double>(pos.z () / 100.0);
	    }
	}
    }
    else 
    {
	std::string error = QString ("### Error: SiStrip Clusters %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisSiStripCluster);
