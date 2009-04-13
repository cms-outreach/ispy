#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewMETTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/IViewSceneGraphService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
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

IViewMETTwig::IViewMETTwig (IgState *state, IgTwig *parent, 
			    const std::string &name)
    : IViewQueuedTwig (state, parent, name)
{}

void
IViewMETTwig::onNewEvent (IViewEventMessage& message) 
{
    IViewQueuedTwig::onNewEvent (message);

    if (IViewReadService *readService = IViewReadService::get (state ()))
    {	
	IViewQWindowService *windowService = IViewQWindowService::get (state ());
	ASSERT (windowService);

	QuarterWidget *viewer = dynamic_cast<QuarterWidget *>(windowService->viewer ());
	SoSeparator *node = dynamic_cast<SoSeparator *>(viewer->getSceneGraph ());

	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName ("IViewMETTwig"));

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xFF5B0000, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

	SoDrawStyle *sty = new SoDrawStyle;
	sty->style = SoDrawStyle::LINES;
	sty->lineWidth.setValue (3);
	sep->addChild (sty);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("METs_V1"))
	{	    
	    IgCollection &mets = storage->getCollection ("METs_V1");
	    if (mets.size () > 0 && mets.hasProperty ("pt") &&
		mets.hasProperty ("px") && 
		mets.hasProperty ("py") &&
		mets.hasProperty ("phi"))
	    {
		SoVertexProperty *vertices = new SoVertexProperty;
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<SbVec3f> points;		

		IgCollectionIterator cit = mets.begin ();
		IgCollectionIterator cend = mets.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;
		    points.push_back (SbVec3f (0., 0., 0.));
		    points.push_back (SbVec3f (item.get<double>("px"), item.get<double>("py"), 0.));

		    lineIndices.push_back (i);
		    lineIndices.push_back (i + 1);
		    lineIndices.push_back (SO_END_LINE_INDEX);
		    i += 2;
		}
		vertices->vertex.setValues (0, points.size (), &points [0]);
		vertices->vertex.setNum (points.size ());	    

		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;

		sep->addChild (lineSet);

		node->addChild (sep);
	    }
	}
    }
}

