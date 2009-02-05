//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisTrack.h"
#include "VisReco/Analyzer/interface/VisLocalPosition.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "DataFormats/GeometrySurface/interface/PlaneBuilder.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
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

VisTrack::VisTrack( const edm::ParameterSet& iPSet )
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
	edm::TypeID collID (typeid (reco::TrackCollection));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void 
VisTrack::analyze( const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisTrack requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<reco::TrackCollection> collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<GlobalTrackingGeometry> geometry;
    eventSetup.get<GlobalTrackingGeometryRecord> ().get (geometry);

    edm::ESHandle<MagneticField> field;    
    eventSetup.get<IdealMagneticFieldRecord> ().get (field);

    if (collection.isValid () && geometry.isValid () && field.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("Tracks_V1");
	IgCollection &tracks = storage->getCollection (text.c_str ());

	IgProperty PT = tracks.addProperty ("pt", static_cast<double>(0.0)); 
	IgProperty X = tracks.addProperty ("x", static_cast<double>(0.0));
	IgProperty Y = tracks.addProperty ("y", static_cast<double>(0.0));
	IgProperty Z = tracks.addProperty ("z", static_cast<double>(0.0));
	IgProperty PX = tracks.addProperty ("px", static_cast<double>(1.0));
	IgProperty PY = tracks.addProperty ("py", static_cast<double>(0.0));
	IgProperty PZ = tracks.addProperty ("pz", static_cast<double>(0.0));
	IgProperty PPHI = tracks.addProperty ("phi", static_cast<double>(0.0));
	IgProperty PETA = tracks.addProperty ("eta", static_cast<double>(0.0));
	IgProperty PCHARGE = tracks.addProperty ("charge", static_cast<double>(0.0));

	// Create a collection for track extras.
	IgCollection &extras = storage->getCollection ("Extras_V1");
	IgProperty IX = extras.addProperty ("ix", static_cast<double>(0.0));
	IgProperty IY = extras.addProperty ("iy", static_cast<double>(0.0));
	IgProperty IZ = extras.addProperty ("iz", static_cast<double>(0.0));
	IgProperty IP_X = extras.addProperty ("ipx", static_cast<double>(1.0));
	IgProperty IP_Y = extras.addProperty ("ipy", static_cast<double>(0.0));
	IgProperty IP_Z = extras.addProperty ("ipz", static_cast<double>(0.0));
	IgProperty OX = extras.addProperty ("ox", static_cast<double>(0.0));
	IgProperty OY = extras.addProperty ("oy", static_cast<double>(0.0));
	IgProperty OZ = extras.addProperty ("oz", static_cast<double>(0.0));
	IgProperty OP_X = extras.addProperty ("opx", static_cast<double>(1.0));
	IgProperty OP_Y = extras.addProperty ("opy", static_cast<double>(0.0));
	IgProperty OP_Z = extras.addProperty ("opz", static_cast<double>(0.0));
 
	IgAssociationSet &trackExtras = storage->getAssociationSet ("TrackExtras_V1");

	// Create a collection for tracking rec hits.
	IgCollection &hits = storage->getCollection ("Hits_V1");

	IgProperty C_X = hits.addProperty ("x", static_cast<double>(0.0));
	IgProperty C_Y = hits.addProperty ("y", static_cast<double>(0.0));
	IgProperty C_Z = hits.addProperty ("z", static_cast<double>(0.0));

	IgAssociationSet &trackHits = storage->getAssociationSet ("TrackHits_V1");
 
	for (reco::TrackCollection::const_iterator track = collection->begin (), trackEnd = collection->end ();
	     track != trackEnd; ++track)
	{
	    IgCollectionItem t = tracks.create ();
	    t ["pt"] = static_cast<double>((*track).pt ());
	    t ["x"] = static_cast<double>((*track).referencePoint ().x () / 100.);
	    t ["y"] = static_cast<double>((*track).referencePoint ().y () / 100.);
	    t ["z"] = static_cast<double>((*track).referencePoint ().z () / 100.);
	    t ["px"] = static_cast<double>((*track).px ());
	    t ["py"] = static_cast<double>((*track).py ());
	    t ["pz"] = static_cast<double>((*track).pz ());
	    t ["phi"] = static_cast<double>((*track).phi ());
	    t ["eta"] = static_cast<double>((*track).eta ());
	    t ["charge"] = static_cast<double>((*track).charge ());
	    
	    if ((*track).innerOk () && (*track).outerOk ())
	    {
		IgCollectionItem e = extras.create ();
		double ix = static_cast<double>((*track).innerPosition ().x () / 100.0);
		double iy = static_cast<double>((*track).innerPosition ().y () / 100.0);
		double iz = static_cast<double>((*track).innerPosition ().z () / 100.0);
		e ["ix"] = static_cast<double>((*track).innerPosition ().x () / 100.0);
		e ["iy"] = static_cast<double>((*track).innerPosition ().y () / 100.0);
		e ["iz"] = static_cast<double>((*track).innerPosition ().z () / 100.0);
		e ["ipx"] = static_cast<double>((*track).innerMomentum ().x ());
		e ["ipy"] = static_cast<double>((*track).innerMomentum ().y ());
		e ["ipz"] = static_cast<double>((*track).innerMomentum ().z ());
		double ox = (*track).outerPosition ().x () / 100.0;
		double oy = (*track).outerPosition ().y () / 100.0;
		double oz = (*track).outerPosition ().z () / 100.0;
		e ["ox"] = static_cast<double>((*track).outerPosition ().x () / 100.0);
		e ["oy"] = static_cast<double>((*track).outerPosition ().y () / 100.0);
		e ["oz"] = static_cast<double>((*track).outerPosition ().z () / 100.0);
		e ["opx"] = static_cast<double>((*track).outerMomentum ().x ());
		e ["opy"] = static_cast<double>((*track).outerMomentum ().y ());
		e ["opz"] = static_cast<double>((*track).outerMomentum ().z ());

		trackExtras.associate (t, e);
	    }
	    
	    if ((*track).extra ().isNonnull ())
	    {
		for (trackingRecHit_iterator it = track->recHitsBegin ();  it != track->recHitsEnd (); ++it) 
		{
		    if ((*it)->isValid () && !(*it)->geographicalId ().null ())
		    {
			IgCollectionItem hit = hits.create ();
			LocalPoint point = VisLocalPosition::localPosition(&(**it), geometry.product());
			double x = geometry->idToDet((*it)->geographicalId())->surface().toGlobal (point).x () / 100.0;
			double y = geometry->idToDet ((*it)->geographicalId ())->surface ().toGlobal (point).y () / 100.0;
			double z = geometry->idToDet ((*it)->geographicalId ())->surface ().toGlobal (point).z () / 100.0;
			hit ["x"] = static_cast<double>(geometry->idToDet((*it)->geographicalId())->surface().toGlobal (point).x () / 100.0);
			hit ["y"] = static_cast<double>(geometry->idToDet ((*it)->geographicalId ())->surface ().toGlobal (point).y () / 100.0);
			hit ["z"] = static_cast<double>(geometry->idToDet ((*it)->geographicalId ())->surface ().toGlobal (point).z () / 100.0);

			trackHits.associate (t, hit);
		    }
		}
	    }
	}
    }
    else 
    {
	std::string error = QString ("### Error: Tracks %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisTrack);
