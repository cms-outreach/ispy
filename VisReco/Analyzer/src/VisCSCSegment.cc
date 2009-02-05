//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisCSCSegment.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <qstring.h>
#include <iostream>
#include <sstream>

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

VisCSCSegment::VisCSCSegment (const edm::ParameterSet& iPSet)
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
	edm::TypeID collID (typeid (CSCSegmentCollection));
	friendlyName_ = collID.friendlyClassName ();	
    }
}

void 
VisCSCSegment::analyze (const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisCSCSegment requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<CSCSegmentCollection> collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<CSCGeometry> geom;
    eventSetup.get<MuonGeometryRecord> ().get (geom);

    if (collection.isValid () && geom.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("CSCSegments_V1");
	IgCollection &icollection = storage->getCollection (text.c_str ());
	IgProperty X1 = icollection.addProperty ("x1", 0.0);
	IgProperty Y1 = icollection.addProperty ("y1", 0.0);
	IgProperty Z1 = icollection.addProperty ("z1", 0.0);
	IgProperty X2 = icollection.addProperty ("x2", 0.0);
	IgProperty Y2 = icollection.addProperty ("y2", 0.0);
	IgProperty Z2 = icollection.addProperty ("z2", 0.0);

	CSCSegmentCollection::const_iterator it = collection->begin ();
	CSCSegmentCollection::const_iterator end = collection->end ();
	for (; it != end; ++it) 
	{
	    IgCollectionItem iitem = icollection.create ();

	    const GeomDet *det = geom->idToDet ((*it).cscDetId ());

	    // Local pos & dir
	    LocalPoint  pos = (*it).localPosition();
	    LocalVector dir = (*it).localDirection();

	    // Chamber thickness (in z)
	    float halfThickness = det->surface ().bounds ().thickness () / 2.;

	    float z1 = halfThickness;
	    float x1 = pos.x() + dir.x()*z1/dir.z();
	    float y1 = pos.y() + dir.y()*z1/dir.z();
	    GlobalPoint g1 = det->surface().toGlobal( LocalPoint(x1,y1,z1) );

	    float z2 = -halfThickness;
	    float x2 = pos.x() + dir.x()*z2/dir.z();
	    float y2 = pos.y() + dir.y()*z2/dir.z();
	    GlobalPoint g2 = det->surface().toGlobal( LocalPoint(x2,y2,z2) );
		
	    float x = g1.x () / 100.0;  // cm -> m
	    float y = g1.y () / 100.0;  // cm -> m
	    float z = g1.z () / 100.0;  // cm -> m

	    iitem ["x1"] = static_cast<double>(x);
	    iitem ["y1"] = static_cast<double>(y);
	    iitem ["z1"] = static_cast<double>(z);
		
	    x = g2.x () / 100.0;  // cm -> m
	    y = g2.y () / 100.0;  // cm -> m
	    z = g2.z () / 100.0;  // cm -> m

	    iitem ["x2"] = static_cast<double>(x);
	    iitem ["y2"] = static_cast<double>(y);
	    iitem ["z2"] = static_cast<double>(z);
	}
    }
    else 
    {
	std::string error = QString ("### Error: CSC Segments %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisCSCSegment);
