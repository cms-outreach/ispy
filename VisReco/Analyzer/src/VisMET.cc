//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisMET.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
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

VisMET::VisMET (const edm::ParameterSet& iPSet)
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
	edm::TypeID collID (typeid (METCollection));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void
VisMET::analyze( const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisMET requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<METCollection> collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    if (collection.isValid ())
    {	
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("METs_V1");
	IgCollection &mets = storage->getCollection (text.c_str ());

	IgProperty PHI = mets.addProperty ("phi", 0.0);
	IgProperty PT = mets.addProperty ("pt", 0.0);
	IgProperty PX = mets.addProperty ("px", 0.0);
	IgProperty PY = mets.addProperty ("py", 0.0);
	IgProperty PZ = mets.addProperty ("pz", 0.0);

	for (METCollection::const_iterator it = collection->begin (), itEnd = collection->end (); it != itEnd; ++it)
	{
	    double phi = (*it).phi ();
	    double pt = (*it).pt ();

	    IgCollectionItem m = mets.create ();
	    m ["phi"] = static_cast<double>(phi);
	    m ["pt"] = static_cast<double>(pt);
	    m ["px"] = static_cast<double>((*it).px ());
	    m ["py"] = static_cast<double>((*it).py ());
	    m ["pz"] = static_cast<double>((*it).pz ());
	}
    }    
    else 
    {
	std::string error = QString ("### Error: MET %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisMET);
