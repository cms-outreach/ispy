#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewSiStripDigiTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/IViewSceneGraphService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Quarter/Quarter.h>
#include <Quarter/QuarterWidget.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace SIM::Coin3D::Quarter;

IViewSiStripDigiTwig::IViewSiStripDigiTwig (IgState *state, IgTwig *parent,
					    const std::string &name /* = "" */)
    : IViewQueuedTwig (state, parent, name)
{}

void
IViewSiStripDigiTwig::onNewEvent (IViewEventMessage& message)
{
    IViewQueuedTwig::onNewEvent (message);
    if (IViewReadService *readService = IViewReadService::get (state ()))
    {	
	IViewQWindowService *windowService = IViewQWindowService::get (state ());
	ASSERT (windowService);

	IViewSceneGraphService *sceneGraphService = IViewSceneGraphService::get (state ());
	ASSERT (sceneGraphService);
	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("SiStripDigis_V1"))
	{	    
	    IgCollection &digis = storage->getCollection ("SiStripDigis_V1");
	    if (digis.size () > 0 && digis.hasProperty ("pos"))
	    {
		IgProperty POS = digis.getProperty ("pos");

		int n = 0;
		SoVertexProperty *vertices = new SoVertexProperty;

		IgCollectionIterator cit = digis.begin ();
		IgCollectionIterator cend = digis.end ();
		for (; cit != cend ; cit++, n++) 
		{
		    IgCollectionItem m = *cit;

		    IgV3d p1 = m.get<IgV3d> (POS);
		
		    double x = p1.x ();
		    double y = p1.y ();
		    double z = p1.z ();
		    vertices->vertex.set1Value (n, SbVec3f (x, y, z));
		}
		vertices->vertex.setNum (n);

		QuarterWidget *viewer = dynamic_cast<QuarterWidget *>(windowService->viewer ());
		SoSeparator *node = dynamic_cast<SoSeparator *>(viewer->getSceneGraph ());

		SoSeparator *sep = new SoSeparator;
		sep->setName (SbName ("SiStripDigis_V1"));

		SoMaterial *mat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0xB0E57C00, rgbcomponents);
		mat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (mat);

		SoMFInt32 tmarkerIndex;
		tmarkerIndex.setValue (SoMarkerSet::PLUS_5_5);
		
		SoMarkerSet *sopoints = new SoMarkerSet;
		sopoints->markerIndex = tmarkerIndex;
		sopoints->vertexProperty.setValue (vertices);
		sopoints->numPoints.setValue (n);
	  
		sep->addChild (sopoints);
		node->addChild (sep);

		// Now SoQt viewer
		SoSeparator *mainScene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
		mainScene->addChild (sep);	

		// 	    SoSeparator *overlayScene = dynamic_cast<SoSeparator *>(sceneGraphService->overlaySceneGraph ());
		// 	    overlayScene->addChild (annot);	
	    }
	}
    }
}


