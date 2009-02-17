#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewSiStripDigiTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/IViewSceneGraphService.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IgHits.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"
#include <Inventor/nodes/SoMaterial.h>
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
{
}

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
	IgCollection &digis = storage->getCollection ("SiStripDigis_V1");
	if (digis.size () > 0)
	{
	    IgColumnHandle xHandle = digis.getHandleByLabel ("x");
	    IgColumnHandle yHandle = digis.getHandleByLabel ("y");
	    IgColumnHandle zHandle = digis.getHandleByLabel ("z");

	    std::vector<QPointF> points;
	    int n = 0;
	    SoVertexProperty *vertices = new SoVertexProperty;

	    IgCollectionIterator cit = digis.begin ();
	    IgCollectionIterator cend = digis.end ();
	    for (; cit != cend ; cit++, n++) 
	    {
		double x = xHandle.get<double> (n);
		double y = yHandle.get<double> (n);
		double z = zHandle.get<double> (n);
		// points.push_back (QPointF (x * 40., y * 40.));
		vertices->vertex.set1Value (n, SbVec3f (x, y, z));
		points.push_back (QPointF (x, y));
	    }
	    vertices->vertex.setNum (n);

	    IggiMainWindow *mainWindow = dynamic_cast<IggiMainWindow *>(windowService->mainWindow ());
	    QGraphicsView *graphicsView = mainWindow->graphicsView;

	    IggiScene *scene = dynamic_cast<IggiScene*>(mainWindow->graphicsView->scene ());
	    IgHits* hits = new IgHits (points);
	    hits->setColor (Qt::green);	    
	    scene->addItem (hits);
	    scene->update ();

	    QuarterWidget *viewer = dynamic_cast<QuarterWidget *>(windowService->viewer ());
	    SoSeparator *node = dynamic_cast<SoSeparator *>(viewer->getSceneGraph ());

	    SoSeparator *sep = new SoSeparator;
	    SoMaterial *mat = new SoMaterial;
	    float rgbcomponents [4];
	    IgSbColorMap::unpack (0x03C03C00, rgbcomponents); // Dark pastel green
	    mat->diffuseColor.setValue (SbColor (rgbcomponents));
	    sep->addChild (mat);

	    SoDrawStyle *sty = new SoDrawStyle;
	    sty->pointSize = 4.0;
	    sep->addChild (sty);

	    SoPointSet *sopoints = new SoPointSet;
	    sopoints->vertexProperty.setValue (vertices);
	    sopoints->numPoints.setValue (n);
	  
	    sep->addChild (sopoints);
	    node->addChild (sep);

	    viewer->viewAll ();

	    // Now SoQt viewer
	    SoSeparator *mainScene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
	    mainScene->addChild (sep);	

// 	    SoSeparator *overlayScene = dynamic_cast<SoSeparator *>(sceneGraphService->overlaySceneGraph ());
// 	    overlayScene->addChild (annot);	
	}
    }
}


