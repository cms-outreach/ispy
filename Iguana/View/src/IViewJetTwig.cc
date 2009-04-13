#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewJetTwig.h"
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

IViewJetTwig::IViewJetTwig (IgState *state, IgTwig *parent, 
			    const std::string &name)
    : IViewQueuedTwig (state, parent, name)
{}

void
IViewJetTwig::onNewEvent (IViewEventMessage& message) 
{
    IViewQueuedTwig::onNewEvent (message);

    IViewQWindowService *windowService = IViewQWindowService::get (state ());
    ASSERT (windowService);

    QuarterWidget *viewer = dynamic_cast<QuarterWidget *>(windowService->viewer ());
    SoSeparator *node = dynamic_cast<SoSeparator *>(viewer->getSceneGraph ());
	
    if (IgDrawFactroyService *drawService = IgDrawFactroyService::get (state ()))
    {	
	SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw ("Jets", state (), "Jets_V1", "3D"));

	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName ("IViewJetTwig"));

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xFF5EAA00, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);
	sep->addChild (rep);
	
	node->addChild (sep);
    }
}
