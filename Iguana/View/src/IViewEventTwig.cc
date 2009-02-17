#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewEventTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/IViewSceneGraphService.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IgAnnotation.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <QString>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QTableWidget>
#include <QTransform>
#include <QWidget>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewEventTwig::IViewEventTwig (IgState *state, IgTwig *parent,
				const std::string &name /* = "" */)
    : IViewQueuedTwig (state, parent, name),
      m_text ("no info")
{
}

void
IViewEventTwig::onNewEvent (IViewEventMessage& message)
{
    IViewQueuedTwig::onNewEvent (message);
    if (IViewReadService *readService = IViewReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
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
	
	    IViewQueuedTwig::onBaseInvalidate ();
	}
    }
    if (IViewQWindowService *windowService = IViewQWindowService::get (state ())) 
    {
	IggiMainWindow *mainWindow = dynamic_cast<IggiMainWindow *>(windowService->mainWindow ());
	QGraphicsView *graphicsView = mainWindow->graphicsView;

	IggiScene *scene = dynamic_cast<IggiScene*>(mainWindow->graphicsView->scene ());
	IgAnnotation *ann = new IgAnnotation (QString (m_text.c_str ()), QPoint (-10, -10), QFont ("Arial", 12, QFont::Bold));
	QPen pen;
	pen.setBrush (Qt::lightGray);	
	ann->setPen (pen);
	scene->addItem (ann);
	scene->update ();
    }
    if (IViewSceneGraphService *sceneGraphService = IViewSceneGraphService::get (state ()))
    {	
	ASSERT (sceneGraphService);

	SoSeparator *overlayScene = dynamic_cast<SoSeparator *>(sceneGraphService->overlaySceneGraph ());
	SoSeparator *sep = new SoSeparator;
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);
    
	SoText2  *eventLabel = new SoText2;
	eventLabel->string = m_text.c_str ();

	SoFont* labelFont = new SoFont;
	labelFont->size.setValue (18.0);
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
    if (IViewQWindowService *windowService = IViewQWindowService::get (state ())) 
    {
	SoSeparator * root = new SoSeparator;
	SoText2 * text = new SoText2;
	SoBaseColor * color = new SoBaseColor;
	SoOrthographicCamera * orthocam = new SoOrthographicCamera;

	text->string.setValue(m_text.c_str ());
	color->rgb.setValue(SbColor(1.0, 0.0, 0.0));

	orthocam->height.setValue(1.0);
	orthocam->nearDistance.setValue(0.0);
	orthocam->farDistance.setValue(2.0);
	orthocam->position = SbVec3f(0.0f, 0.0f, 1.0f);

	SoTranslation *eventLabelTranslation = new SoTranslation;
    
	SbVec3f pos = SbVec3f (-0.7,
			       0.4,
			       0.0);

	eventLabelTranslation->translation = pos;

	root->addChild(orthocam);
	root->addChild(color);
	root->addChild(eventLabelTranslation);
	root->addChild(text);

	SoRenderManager::Superimposition *super = windowService->viewer ()->getSoRenderManager()->addSuperimposition(root);
	windowService->super (super);
    }
    
}
