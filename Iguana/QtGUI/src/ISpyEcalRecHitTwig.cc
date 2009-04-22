#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyEcalRecHitTwig.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
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

ISpyEcalRecHitTwig::ISpyEcalRecHitTwig (IgState *state, IgTwig *parent, 
					const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyEcalRecHitTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("EcalRecHits_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;
    
    if (IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ()))
    {	
	SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw ("CrystalHits", state (), "EcalRecHits_V1", "3D"));
    
	SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());	
	sep->setName (SbName ("ISpyEcalRecHitTwig"));

	SoMaterial *mat = new SoMaterial;
	//float rgbcomponents [4];
	//IgSbColorMap::unpack (0xCC155900, rgbcomponents);
	//IgSbColorMap::unpack(0xff009c00, rgbcomponents);
	//mat->diffuseColor.setValue (SbColor (rgbcomponents));
	mat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);

	sep->addChild (mat);
	sep->addChild (rep);
    }
}

