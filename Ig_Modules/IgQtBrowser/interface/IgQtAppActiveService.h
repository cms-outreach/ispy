#ifndef IG_QT_BROWSER_IG_APP_ACTIVE_SERVICE_H
# define IG_QT_BROWSER_IG_APP_ACTIVE_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgDispatcher.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_QT_BROWSER_API IgQtAppActiveMessage
{
public:
    IgQtAppActiveMessage (QWidget *widget, bool active)
      : m_widget (widget),
        m_active  (active)
      { }
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    QWidget*		widget (void) const
    { return m_widget; }
    
    bool		active (void) const
    { return m_active; }

private:
    QWidget *		m_widget;
    bool		m_active;
};

class IG_QT_BROWSER_API IgQtAppActiveService
    : public IgStateElement, public IgDispatcher<IgQtAppActiveMessage>
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppActiveService);
public:
    IgQtAppActiveService (IgState *state);

private:
    IgState		*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_APP_ACTIVE_SERVICE_H
