#ifndef IG_QT_BROWSER_IG_QT_SCHEDULED_EVENT_H
# define IG_QT_BROWSER_IG_QT_SCHEDULED_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include <classlib/utils/Callback.h>
# include <qevent.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtScheduledEvent : public QCustomEvent
{
public:
    static const int EVENT_ID = QEvent::User + 1;
    IgQtScheduledEvent (lat::Callback action);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    lat::Callback callback (void);
private:
    lat::Callback 	m_callback;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_SCHEDULED_EVENT_H
