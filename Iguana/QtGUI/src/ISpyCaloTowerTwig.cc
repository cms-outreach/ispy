#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyCaloTowerTwig.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
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

ISpyCaloTowerTwig::ISpyCaloTowerTwig (IgState *state, IgTwig *parent, 
				      const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyCaloTowerTwig::onNewEvent (ISpyEventMessage& message) 
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("CaloTowers_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ()))
    {	
	SoSeparator *emrep = dynamic_cast<SoSeparator *>(drawService->draw ("EmTowers", state (), "CaloTowers_V1", "3D"));
	SoSeparator *hadrep = dynamic_cast<SoSeparator *>(drawService->draw ("HadTowers", state (), "CaloTowers_V1", "3D"));

	SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
	sep->setName (SbName ("ISpyCaloTowerTwig"));

	SoMaterial *emat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xB0E50000, rgbcomponents);
	emat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (emat);
	sep->addChild (emrep);

	SoMaterial *hmat = new SoMaterial;
	IgSbColorMap::unpack (0x00E57C00, rgbcomponents);
	hmat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (hmat);
	sep->addChild (hadrep);
    }
   
}

