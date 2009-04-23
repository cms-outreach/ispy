#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyHERecHitTwig.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/QtGUI/interface/debug.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <QSettings>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyHERecHitTwig::ISpyHERecHitTwig (IgState *state, IgTwig *parent, 
				    const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyHERecHitTwig::onNewEvent (ISpyEventMessage& message) 
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("HERecHits_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ()))
    {	
	SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw ("CrystalHits", state (), "HERecHits_V1", "3D"));
    
	SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());	
	sep->setName (SbName ("ISpyHERecHitTwig"));

	SoMaterial *mat = new SoMaterial;
	//float rgbcomponents [4];
	//IgSbColorMap::unpack (0x21B6A800, rgbcomponents);
	//mat->diffuseColor.setValue (SbColor (rgbcomponents));
	mat->diffuseColor.setValue(0.0, 0.4, 1.0);
	sep->addChild (mat);
	sep->addChild (rep);
    }
}
