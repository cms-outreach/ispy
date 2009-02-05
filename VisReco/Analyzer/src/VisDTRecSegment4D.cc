//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisDTRecSegment4D.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
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

VisDTRecSegment4D::VisDTRecSegment4D (const edm::ParameterSet& iPSet)
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
	edm::TypeID collID (typeid (DTRecSegment4DCollection));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void 
VisDTRecSegment4D::analyze (const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisDTRecSegment4D requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<DTRecSegment4DCollection> collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<DTGeometry> geom;
    eventSetup.get<MuonGeometryRecord> ().get (geom);

    if (collection.isValid () && geom.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("DTRecSegment4D_V1");
	IgCollection &segments = storage->getCollection (text.c_str ());

	IgProperty X1 = segments.addProperty ("x1", static_cast<double>(0.0));
	IgProperty Y1 = segments.addProperty ("y1", static_cast<double>(0.0));
	IgProperty Z1 = segments.addProperty ("z1", static_cast<double>(0.0));
	IgProperty X2 = segments.addProperty ("x2", static_cast<double>(0.0));
	IgProperty Y2 = segments.addProperty ("y2", static_cast<double>(0.0));
	IgProperty Z2 = segments.addProperty ("z2", static_cast<double>(0.0));

	DTRecSegment4DCollection::const_iterator it = collection->begin ();
	DTRecSegment4DCollection::const_iterator end = collection->end ();
	for (; it != end; ++it) 
	{
	    DTChamberId chId ((*it).geographicalId ().rawId ());
	    const DTChamber *chamber = geom->chamber (chId);

	    float halfHeight = chamber->surface ().bounds ().thickness () / 2.0;
	    // float halfWidth = chamber->surface ().bounds ().width () / 2.0;
	    LocalVector locDir = (*it).localDirection ();
	    LocalPoint locPos = (*it).localPosition ();

	    GlobalPoint posInner = chamber->toGlobal (locPos + locDir / locDir.mag () * halfHeight / cos (locDir.theta ()));		  
	    float x = posInner.x () / 100.0;  // cm -> m
	    float y = posInner.y () / 100.0;  // cm -> m
	    float z = posInner.z () / 100.0;  // cm -> m

	    IgCollectionItem s = segments.create ();
	    s ["x1"] = static_cast<double>(x);
	    s ["y1"] = static_cast<double>(y);
	    s ["z1"] = static_cast<double>(z);

	    GlobalPoint posOuter = chamber->toGlobal (locPos + locDir / (-locDir.mag ()) * halfHeight / cos (locDir.theta ()));		  
	    x = posOuter.x () / 100.0;  // cm -> m
	    y = posOuter.y () / 100.0;  // cm -> m
	    z = posOuter.z () / 100.0;  // cm -> m

	    s ["x2"] = static_cast<double>(x);
	    s ["y2"] = static_cast<double>(y);
	    s ["z2"] = static_cast<double>(z);
	}
    }
    else 
    {
	std::string error = QString ("### Error: DTRecSegment4Ds %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisDTRecSegment4D);
