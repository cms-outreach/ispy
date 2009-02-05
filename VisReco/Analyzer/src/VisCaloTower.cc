//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/Analyzer/interface/VisCaloTower.h"
#include "VisFramework/VisFrameworkBase/interface/IguanaService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Utilities/classlib/utils/DebugAids.h"
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

VisCaloTower::VisCaloTower (const edm::ParameterSet& iPSet)
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
	edm::TypeID collID (typeid (CaloTowerCollection));
	friendlyName_ = collID.friendlyClassName ();
    }
}

void
VisCaloTower::analyze( const edm::Event& event, const edm::EventSetup& eventSetup)
{
    edm::Service<IguanaService> config;
    if (! config.isAvailable ()) 
    {
	throw cms::Exception ("Configuration")
	    << "VisCaloTower requires the IguanaService\n"
	    "which is not present in the configuration file.\n"
	    "You must add the service in the configuration file\n"
	    "or remove the module that requires it";
    }
    
    edm::Handle<CaloTowerCollection> collection;
    VisEventSelector visSel (friendlyName_, moduleLabel_, instanceName_, processName_);
    event.get (visSel, collection);

    edm::ESHandle<CaloGeometry> geom;
    eventSetup.get<CaloGeometryRecord> ().get (geom);

    if (collection.isValid () && geom.isValid ())
    {	    
	IgDataStorage *storage = config->storage ();
	std::string text = QString("CaloTowers_V1");
	IgCollection &caloTowers = storage->getCollection(text.c_str());
	IgProperty ET = caloTowers.addProperty("et", 0.0);
	IgProperty ETA = caloTowers.addProperty("eta", 0.0);
	IgProperty PHI = caloTowers.addProperty("phi", 0.0);
	IgProperty IPHI = caloTowers.addProperty("iphi", 0.0);
	IgProperty HE = caloTowers.addProperty("hadEnergy", 0.0);
	IgProperty EE = caloTowers.addProperty("emEnergy", 0.0);
	IgProperty POS = caloTowers.addProperty("pos", IgV3d());

	IgCollection &icorners = storage->getCollection("Corners_V1");
	IgProperty FRONT_1 = icorners.addProperty("front_1", IgV3d());
	IgProperty FRONT_2 = icorners.addProperty("front_2", IgV3d());
	IgProperty FRONT_3 = icorners.addProperty("front_3", IgV3d());
	IgProperty FRONT_4 = icorners.addProperty("front_4", IgV3d());
	IgProperty BACK_1 = icorners.addProperty("back_1", IgV3d());
	IgProperty BACK_2 = icorners.addProperty("back_2", IgV3d());
	IgProperty BACK_3 = icorners.addProperty("back_3", IgV3d());
	IgProperty BACK_4 = icorners.addProperty("back_4", IgV3d());

	IgAssociationSet &caloTowersCorners = storage->getAssociationSet("CaloTowersCorners_V1");
	
	for (CaloTowerCollection::const_iterator it=collection->begin(), itEnd=collection->end(); it!=itEnd; ++it)
	{
	    const CaloCellGeometry *cell = (*geom).getGeometry((*it).id());

	    const CaloCellGeometry::CornersVec& corners = cell->getCorners();
            ASSERT(corners.size()==8);

	    IgCollectionItem itower = caloTowers.create();
	    itower["et"] = static_cast<double>((*it).et());
	    itower["eta"] = static_cast<double>((*it).eta());
	    itower["phi"] = static_cast<double>((*it).phi());
	    itower["iphi"] = static_cast<double>((*it).iphi());
	    itower["hadEnergy"] = static_cast<double>((*it).hadEnergy());
	    itower["emEnergy"] = static_cast<double>((*it).emEnergy());
	    
	    IgCollectionItem icorner = icorners.create();
	    icorner["front_1"] = IgV3d(static_cast<double>(corners[0].x()/100.0), static_cast<double>(corners[0].y()/100.0), static_cast<double>(corners[0].z()/100.0));
	    icorner["front_2"] = IgV3d(static_cast<double>(corners[1].x()/100.0), static_cast<double>(corners[1].y()/100.0), static_cast<double>(corners[1].z()/100.0));
	    icorner["front_3"] = IgV3d(static_cast<double>(corners[2].x()/100.0), static_cast<double>(corners[2].y()/100.0), static_cast<double>(corners[2].z()/100.0));
	    icorner["front_4"] = IgV3d(static_cast<double>(corners[3].x()/100.0), static_cast<double>(corners[3].y()/100.0), static_cast<double>(corners[3].z()/100.0));
	    icorner["back_1"] = IgV3d(static_cast<double>(corners[4].x()/100.0), static_cast<double>(corners[4].y()/100.0), static_cast<double>(corners[4].z()/100.0));
	    icorner["back_2"] = IgV3d(static_cast<double>(corners[5].x()/100.0), static_cast<double>(corners[5].y()/100.0), static_cast<double>(corners[5].z()/100.0));
	    icorner["back_3"] = IgV3d(static_cast<double>(corners[6].x()/100.0), static_cast<double>(corners[6].y()/100.0), static_cast<double>(corners[6].z()/100.0));
	    icorner["back_4"] = IgV3d(static_cast<double>(corners[7].x()/100.0), static_cast<double>(corners[7].y()/100.0), static_cast<double>(corners[7].z()/100.0));

	    caloTowersCorners.associate(itower,icorner);		
	}
    }
    else 
    {
	std::string error = QString ("### Error: CaloTowers %1:%2:" // friendlyName:moduleLabel:instanceName:processName
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

DEFINE_FWK_MODULE(VisCaloTower);
