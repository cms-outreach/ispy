#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyEventTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <QString>
#include <QSettings>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
void
createTextLine(SoGroup *group, SoTranslation *trans, const std::string &text)
{
    SoText2 *label = new SoText2;
    label->string = text.c_str ();
    group->addChild (trans);
    group->addChild (label);
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyEventTwig::ISpyEventTwig (IgState *state, IgTwig *parent,
				const std::string &name /* = "" */)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyEventTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("Event_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("Event_V1"))
	{	    
	    IgCollection &event = storage->getCollection ("Event_V1");
	    if (event.size () > 0)
	    {	    
		IgCollectionIterator it = event.begin();
		IgCollectionItem eventId = *it;

                m_time  = (QString ("Data recorded  %1").arg (eventId.get<std::string>("time").c_str ()).toStdString ());
                m_run   = (QString ("Run number     %1").arg (eventId.get<int>("run"))  .toStdString ());
                m_event = (QString ("Event number   %1").arg (eventId.get<int>("event")).toStdString ());
                m_ls    = (QString ("Lumi section   %1").arg (eventId.get<int>("ls"))   .toStdString ());
                m_orbit = (QString ("Orbit number   %1").arg (eventId.get<int>("orbit")).toStdString ());
                m_bx    = (QString ("Beam crossing  %1").arg (eventId.get<int>("bx"))   .toStdString ());

	    }
	}	
    }

    SoSeparator *top = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
    SoAnnotation *overlayScene = new SoAnnotation;
    top->addChild (overlayScene);

    SoOrthographicCamera *camera = new SoOrthographicCamera;
    camera->nearDistance = 1;
    camera->farDistance = 10;
    camera->pointAt (SbVec3f(0.0, 0.0, 0.0));
    camera->scaleHeight (5.5f);
    camera->focalDistance = 1;
    overlayScene->addChild (camera);

    SoSeparator *sep = new SoSeparator;

    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (1.0, 1.0, 1.0);
    sep->addChild (mat);

    SoFont *labelFont = new SoFont;
    labelFont->size.setValue (13.0);
    labelFont->name.setValue ("Courier"); // LT FIXME: fixed width font used in formatting. Should layout a proper table one day.
    sep->addChild (labelFont);

    SoTranslation *textStartTranslation = new SoTranslation;
    SoTranslation *nextLineTranslation  = new SoTranslation;

    textStartTranslation->translation = SbVec3f (-5.0,  5.0,  0.0);
    nextLineTranslation ->translation = SbVec3f ( 0.0, -0.25, 0.0);

    createTextLine (sep, textStartTranslation, 
                    "-- iSpy -- http://iguana.cern.ch/ispy");

    createTextLine (sep, nextLineTranslation, m_time);
    createTextLine (sep, nextLineTranslation, m_run);
    createTextLine (sep, nextLineTranslation, m_event);
    createTextLine (sep, nextLineTranslation, m_ls);
    createTextLine (sep, nextLineTranslation, m_orbit);
    createTextLine (sep, nextLineTranslation, m_bx);

    overlayScene->addChild (sep);
}
