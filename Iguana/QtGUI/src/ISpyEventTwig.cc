#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyEventTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
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

ISpyEventTwig::ISpyEventTwig (IgState *state, IgTwig *parent,
				const std::string &name /* = "" */)
    : ISpyQueuedTwig (state, parent, name),
      m_text ("no info")
{}

void
ISpyEventTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("Event_V1"))
	{	    
	    IgCollection &event = storage->getCollection ("Event_V1");
	    if (event.size () > 0)
	    {	    
		IgColumnHandle runHandle = event.getHandleByLabel ("run");
		IgColumnHandle eventHandle = event.getHandleByLabel ("event");
		IgColumnHandle lsHandle = event.getHandleByLabel ("ls");
		IgColumnHandle orbitHandle = event.getHandleByLabel ("orbit");
		IgColumnHandle bxHandle = event.getHandleByLabel ("bx");
		IgColumnHandle timeHandle = event.getHandleByLabel ("time");

		IgCollectionIterator it = event.begin();
		IgCollectionItem eventId = *it;

		m_text = (QString ("%1 GMT: Run %2, Event %3, LS %4, Orbit %5, BX %6")
			  .arg (eventId.get<std::string>("time").c_str ())
			  .arg (eventId.get<int>("run"))
			  .arg (eventId.get<int>("event"))
			  .arg (eventId.get<int>("ls"))
			  .arg (eventId.get<int>("orbit"))
			  .arg (eventId.get<int>("bx"))
			  .toStdString ());
	    }
	}	
    }

    SoSeparator *top = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
    SoAnnotation *overlayScene = new SoAnnotation;
    top->addChild (overlayScene);
    
    SoPerspectiveCamera *pcam = new SoPerspectiveCamera;
    pcam->position = SbVec3f (0, 0, 5);
    pcam->nearDistance = 0.1;
    pcam->farDistance = 10;
    overlayScene->addChild (pcam);

    SoSeparator *sep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);
    
    SoText2 *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();
    
    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (14.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);
    
    SoTranslation *eventLabelTranslation = new SoTranslation;
    
    SbVec3f pos = SbVec3f (-2.5,
			   1.65,
			   0.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);
    
    overlayScene->addChild (sep);
}
