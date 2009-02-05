//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtScheduledEventExecutor.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtScheduledEvent.h"
#include <qevent.h>
#include <qobject.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtScheduledEventExecutor::IgQtScheduledEventExecutor (void)
    : QObject ()
{
}

void
IgQtScheduledEventExecutor::customEvent (QCustomEvent *event)
{
    IgQtScheduledEvent * scheduledEvent 
	= dynamic_cast <IgQtScheduledEvent *> (event);
    ASSERT (scheduledEvent);    
    (scheduledEvent->callback ()) ();
}
