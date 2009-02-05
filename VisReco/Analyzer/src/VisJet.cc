//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisJet.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
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

VisJet::VisJet (const edm::ParameterSet& iPSet)
    : friendlyName_( std::string() ),
      moduleLabel_( std::string() ),
      instanceName_( std::string() ),
      processName_( std::string() ),
      energyCut_( 0.1 )
{
    friendlyName_ = iPSet.getUntrackedParameter<std::string>( "friendlyName", std::string());
    moduleLabel_ = iPSet.getUntrackedParameter<std::string>( "moduleLabel", std::string());
    instanceName_ = iPSet.getUntrackedParameter<std::string>( "instanceName", std::string());
    processName_ = iPSet.getUntrackedParameter<std::string>( "processName", std::string());
    energyCut_ = iPSet.getUntrackedParameter<double>( "energyCut", 0.1);
}

void
VisJet::analyze( const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisJet requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<reco::CaloJetCollection> collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);
    
    std::vector<edm::Handle<CaloTowerCollection> > towerCollections;
    event.getManyByType (towerCollections);

    if (collection.isValid ())
    {	
	IgDataStorage *storage = config->storage ();
	std::string text = QString ("Jets_V1");
	IgCollection &jets = storage->getCollection (text.c_str ());

	IgProperty ET = jets.addProperty ("et", 0.0); 
	IgProperty ETA = jets.addProperty ("eta", 0.0);
	IgProperty THETA = jets.addProperty ("theta", 0.0);
	IgProperty PHI = jets.addProperty ("phi", 0.0);

	for (reco::CaloJetCollection::const_iterator it = collection->begin (), itEnd = collection->end (); it != itEnd; ++it)
	{
	    IgCollectionItem ijet = jets.create ();
	    ijet["et"] = static_cast<double>((*it).et ());
	    ijet["eta"] = static_cast<double>((*it).eta ());
	    ijet["theta"] = static_cast<double>(2.0 * atan (exp (-(*it).eta ())));
	    ijet["phi"] = static_cast<double>((*it).phi());
	}
    }    
    else 
    {
	std::string error = QString ("### Error: Jets %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisJet);
