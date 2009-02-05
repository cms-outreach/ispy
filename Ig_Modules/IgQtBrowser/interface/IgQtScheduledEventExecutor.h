#ifndef IG_QT_BROWSER_IG_QT_SCHEDULED_EVENT_EXECUTOR_H
# define IG_QT_BROWSER_IG_QT_SCHEDULED_EVENT_EXECUTOR_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QCustomEvent;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtScheduledEventExecutor : public QObject
{
public:
    IgQtScheduledEventExecutor (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
protected:
    void customEvent  (QCustomEvent *event);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_SCHEDULED_EVENT_EXECUTOR_H
