#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyJetTwig.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/QtGUI/interface/ISpySceneGraphService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyJetTwig::ISpyJetTwig (IgState *state, IgTwig *parent, 
			  const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyJetTwig::onNewEvent (ISpyEventMessage& message) 
{
    ISpyQueuedTwig::onNewEvent (message);

    ISpySceneGraphService *sceneGraphService = ISpySceneGraphService::get (state ());
    ASSERT (sceneGraphService);

    if (IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ()))
    {	
	SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw ("Jets", state (), "Jets_V1", "3D"));

	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName ("ISpyJetTwig"));

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xFF5EAA00, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);
	sep->addChild (rep);

	SoSeparator *mainScene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
	ASSERT (mainScene);	
	mainScene->addChild (sep);	
    }
}
