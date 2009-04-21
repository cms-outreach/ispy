//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyEventFilter.h"
#include <QEvent>
#include <QFileOpenEvent>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

bool
ISpyEventFilter::eventFilter (QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FileOpen)
    {	
	QFileOpenEvent *foe = static_cast<QFileOpenEvent *> (event);
	open (foe->file());
	qWarning ("QFileOpenEvent %d", event->type ());
	return true;
    }
    else
    {
	// standard event processing
	return QObject::eventFilter (obj, event);
    }
}
