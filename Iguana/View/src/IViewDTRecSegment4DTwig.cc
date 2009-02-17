#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewDTRecSegment4DTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/IViewSceneGraphService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IgLine.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <QString>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewDTRecSegment4DTwig::IViewDTRecSegment4DTwig (IgState *state, IgTwig *parent,
						  const std::string &name /* = "" */)
    : IViewQueuedTwig (state, parent, name)
{}

void
IViewDTRecSegment4DTwig::onNewEvent (IViewEventMessage& message)
{
    IViewQueuedTwig::onNewEvent (message);
    if (IViewReadService *readService = IViewReadService::get (state ()))
    {	
	IViewQWindowService *windowService = IViewQWindowService::get (state ());
	ASSERT (windowService);
	
	IgDataStorage *storage = readService->dataStorage ();
	IgCollection &segments = storage->getCollection ("DTRecSegment4D_V1");
	if (segments.size () > 0)
	{    
	    IgColumnHandle x1Handle = segments.getHandleByLabel ("x1");
	    IgColumnHandle y1Handle = segments.getHandleByLabel ("y1");
	    IgColumnHandle z1Handle = segments.getHandleByLabel ("z1");
	    IgColumnHandle x2Handle = segments.getHandleByLabel ("x2");
	    IgColumnHandle y2Handle = segments.getHandleByLabel ("y2");
	    IgColumnHandle z2Handle = segments.getHandleByLabel ("z2");

	    IggiMainWindow *mainWindow = dynamic_cast<IggiMainWindow *>(windowService->mainWindow ());
	    QGraphicsView *graphicsView = mainWindow->graphicsView;
	    IggiScene *scene = dynamic_cast<IggiScene*>(mainWindow->graphicsView->scene ());

	    IViewSceneGraphService *sceneGraphService = IViewSceneGraphService::get (state ());	    
	    ASSERT (sceneGraphService);

	    SoSeparator *soscene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
	    SoSeparator *sep = new SoSeparator;
	    SoMaterial *mat = new SoMaterial;
	    float rgbcomponents [4];
	    IgSbColorMap::unpack (0xC0000000, rgbcomponents); // Free Speech Red
	    mat->diffuseColor.setValue (SbColor (rgbcomponents));
	    sep->addChild (mat);
    
	    //set line width
	    SoDrawStyle *sty = new SoDrawStyle;
	    sty->style = SoDrawStyle::LINES;
	    sty->lineWidth.setValue (3);
	    sep->addChild (sty);

	    int n = 0;
	    IgCollectionIterator cit = segments.begin ();
	    IgCollectionIterator cend = segments.end ();
	    for (; cit != cend ; cit++, n++) 
	    {
		SoVertexProperty *vtx = new SoVertexProperty;
		
		SoLineSet *soline = new SoLineSet;
		soline->numVertices = 2;

		double x1 = x1Handle.get<double> (n);
		double y1 = y1Handle.get<double> (n);
		double z1 = z1Handle.get<double> (n);    
		
		double x2 =  x2Handle.get<double> (n);
		double y2 =  y2Handle.get<double> (n);
		double z2 =  z2Handle.get<double> (n);
		// IgLine *line = new IgLine (QPointF (x1 * 40., y1 * 40.), QPointF (x2 * 40., y2 * 40.));
		IgLine *line = new IgLine (QPointF (x1, y1), QPointF (x2, y2));
		line->setColor (Qt::red);
		
		scene->addItem (line);

		vtx->vertex.set1Value (0, SbVec3f (x1, y1, z1));
		vtx->vertex.set1Value (1, SbVec3f (x2, y2, z2));
		soline->vertexProperty = vtx;
		sep->addChild (soline);
	    }
	    scene->update ();		
	    soscene->addChild (sep);
	}
    }
}

