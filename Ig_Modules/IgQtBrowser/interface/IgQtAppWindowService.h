#ifndef IG_QT_BROWSER_IG_QT_APP_WINDOW_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_WINDOW_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppWindowService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppWindowService);
public:
    IgQtAppWindowService (IgState *state, QWidget *mainWindow);
    ~IgQtAppWindowService (void);

    virtual QWidget *		mainWindow (void);

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;

    // undefined semantics
    IgQtAppWindowService (const IgQtAppWindowService &);
    IgQtAppWindowService &operator= (const IgQtAppWindowService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_WINDOW_SERVICE_H
