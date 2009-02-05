#ifndef IG_QT_BROWSER_IG_QT_APP_CONTEXT_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_CONTEXT_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include "classlib/utils/Callback.h"
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class lat::CallbackRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppContextService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppContextService);
public:
    typedef void (*ConnectionCallback) (void *data, bool focus);    

    IgQtAppContextService (IgState *state, QWidget *mainWindow);
    ~IgQtAppContextService (void);

    void add (ConnectionCallback connection, 
	      void *data, bool enable = true);

    void addOnFocusIn (lat::CallbackRep *callback);
    void addOnFocusOut (lat::CallbackRep *callback);
            
    void focusIn (void);
    void focusOut (void);
    
    // FIXME: methods to manage/switch current user attention
    // context; use in pre-menu-popups to enable/disable items,
    // to switch context menu contents, and to determine what
    // window/context menu items like "Print" should apply to.

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;
    
    typedef std::list<lat::Callback> CallbackList;

    CallbackList		m_focusInCallbacks;
    CallbackList		m_focusOutCallbacks;    
    
    // undefined semantics
    IgQtAppContextService (const IgQtAppContextService &);
    IgQtAppContextService &operator= (const IgQtAppContextService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_CONTEXT_SERVICE_H
