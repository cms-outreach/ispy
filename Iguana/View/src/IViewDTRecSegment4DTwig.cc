#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewDTRecSegment4DTwig.h"
#include "Iguana/View/interface/IgDrawFactroyService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
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

IViewDTRecSegment4DTwig::IViewDTRecSegment4DTwig (IgState *state, IgTwig *parent,
						  const std::string &name /* = "" */)
    : IViewQueuedTwig (state, parent, name)
{}

void
IViewDTRecSegment4DTwig::onNewEvent (IViewEventMessage& message)
{
    IViewQueuedTwig::onNewEvent (message);

    IViewQWindowService *windowService = IViewQWindowService::get (state ());
    ASSERT (windowService);

    QuarterWidget *viewer = dynamic_cast<QuarterWidget *>(windowService->viewer ());
    SoSeparator *node = dynamic_cast<SoSeparator *>(viewer->getSceneGraph ());
    
    if (IgDrawFactroyService *drawService = IgDrawFactroyService::get (state ()))
    {	
	SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw ("Segments", state (), "DTRecSegment4D_V1", "3D"));

	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName ("IViewDTRecSegment4DTwig"));

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
	
	sep->addChild (rep);
	
	node->addChild (sep);
    }
}
