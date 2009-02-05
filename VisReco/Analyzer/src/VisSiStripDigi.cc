//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisSiStripDigi.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
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

VisSiStripDigi::VisSiStripDigi (const edm::ParameterSet& iPSet)
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
	edm::TypeID collID (typeid (edm::DetSetVector<SiStripDigi>));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void 
VisSiStripDigi::analyze( const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisSiStripDigi requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<edm::DetSetVector<SiStripDigi> > collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<TrackerGeometry> geom;
    eventSetup.get<TrackerDigiGeometryRecord> ().get (geom);

    if (collection.isValid () && geom.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("SiStripDigis_V1");
	IgCollection &digis = storage->getCollection (text.c_str ());

	IgProperty X = digis.addProperty ("x", 0.0);
	IgProperty Y = digis.addProperty ("y", 0.0);
	IgProperty Z = digis.addProperty ("z", 0.0);

	edm::DetSetVector<SiStripDigi>::const_iterator it = collection->begin ();
	edm::DetSetVector<SiStripDigi>::const_iterator end = collection->end ();

	for (; it != end; ++it)
	{
	    edm::DetSet<SiStripDigi> ds = *it;

	    if (ds.data.size ())
	    {
		const uint32_t& detID = ds.id;
		DetId detid (detID);
		edm::DetSet<SiStripDigi>::const_iterator idigi = ds.data.begin ();
		edm::DetSet<SiStripDigi>::const_iterator idigiEnd = ds.data.end ();
		for(; idigi != idigiEnd; ++idigi)
		{ 
		    const StripGeomDetUnit* stripDet = dynamic_cast<const StripGeomDetUnit *>(geom->idToDet (detid));
		    const StripTopology* stripTopol = dynamic_cast<const StripTopology *>( &(stripDet->specificTopology ()));

		    GlobalPoint pos = (geom->idToDet (detid))->surface ().toGlobal (stripTopol->localPosition ((*idigi).strip ()));
		    IgCollectionItem d = digis.create ();
		    d ["x"] = static_cast<double>(pos.x () / 100.);
		    d ["y"] = static_cast<double>(pos.y () / 100.);
		    d ["z"] = static_cast<double>(pos.z () / 100.);
		}
	    }
	}
    }
    else 
    {
	std::string error = QString ("### Error: SiStrip Digis %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisSiStripDigi);
