#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyMuonTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyMuonTwig::ISpyMuonTwig (IgState *state, IgTwig *parent,
			    const std::string &name /* = "" */)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyMuonTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("Muons_V1"))
	{	    
	    IgCollection &muons = storage->getCollection ("Muons_V1");
	    if (muons.size () > 0)
	    {    
		IgColumnHandle ptHandle = muons.getHandleByLabel ("pt");
		IgColumnHandle chargeHandle = muons.getHandleByLabel ("charge");
		IgColumnHandle rpHandle = muons.getHandleByLabel ("rp");
		IgColumnHandle phiHandle = muons.getHandleByLabel ("phi");
		IgColumnHandle etaHandle = muons.getHandleByLabel ("eta");

		// IgAssociationSet &muonDetIds = storage->getAssociationSet ("MuonDetIds_V1");
		// IgCollection &detIds = storage->getCollection ("DetIds_V1");
		IgAssociationSet &muonTrackerPoints = storage->getAssociationSet ("MuonTrackerPoints_V1");
		// IgAssociationSet &muonStandalonePoints = storage->getAssociationSet ("MuonStandalonePoints_V1");
		// IgAssociationSet &muonGlobalPoints = storage->getAssociationSet ("MuonGlobalPoints_V1");
		IgCollection &points = storage->getCollection ("Points_V1");

		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
		SoMaterial *mat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0xffff0000, rgbcomponents);
		mat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (mat);

		IgCollectionIterator cit = muons.begin ();
		IgCollectionIterator cend = muons.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem imuon = *cit;
		    // double pt = imuon.get<double> ("pt");

		    IgSoSimpleTrajectory* reconstructedTrack = new IgSoSimpleTrajectory;
		    reconstructedTrack->lineWidth = 3.0;

		    int n = 0;
		    for (IgAssociationSet::Iterator ha = muonTrackerPoints.begin ();
			 ha != muonTrackerPoints.end ();
			 ++ha)
		    {
			if (ha->first ().objectId () == imuon.currentRow ())
			{
			    IgCollectionItem hm (&points, ha->second ().objectId ());
			    double x = hm.get<IgV3d>("pos").x ();
			    double y = hm.get<IgV3d>("pos").y ();
			    double z = hm.get<IgV3d>("pos").z ();
			    reconstructedTrack->controlPoints.set1Value (n, SbVec3f(x, y, z));
			    reconstructedTrack->markerPoints.set1Value (n, SbVec3f(x, y, z));
			    n++;			
			}
		    }
		    sep->addChild (reconstructedTrack);
		}
	    }
	}	
    }
}
