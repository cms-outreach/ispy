#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewEventTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/Ig3DRep.h"
#include "Iguana/View/interface/IgRPhiRep.h"
#include "Iguana/View/interface/IgRZRep.h"
#include "Iguana/View/interface/IgLegoRep.h"
#include "Iguana/View/interface/IgTextRep.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IgAnnotation.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"
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
	IgAnnotation *ann = new IgAnnotation (QString (m_text.c_str ()), QPoint (-290, -190), QFont ("Arial", 12, QFont::Bold));
	QPen pen;
	pen.setBrush (Qt::lightGray);	
	ann->setPen (pen);
	scene->addItem (ann);
	scene->update ();
    }
}

void
IViewEventTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    rep->setText (m_text);
}

void
IViewEventTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}

void
IViewEventTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}

void
IViewEventTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}

void
IViewEventTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}
