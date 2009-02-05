#ifndef IG_QT_BROWSER_IG_QT_APP_CRASH_ALERT_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_CRASH_ALERT_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <classlib/utils/Callback.h>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppCrashAlertService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppCrashAlertService);
public:
    IgQtAppCrashAlertService (IgState *state, QWidget *mainWindow);
    ~IgQtAppCrashAlertService (void);

    virtual bool		hasCrashed (void);
    virtual void		addUnwind (lat::Callback action);
    virtual void		removeUnwind (lat::Callback action);

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;
    QWidget			*m_popup;
    std::list<lat::Callback>	m_unwinds;

    // undefined semantics
    IgQtAppCrashAlertService (const IgQtAppCrashAlertService &);
    IgQtAppCrashAlertService &operator= (const IgQtAppCrashAlertService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_CRASH_ALERT_SERVICE_H
