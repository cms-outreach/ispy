#ifndef IG_QT_BROWSER_IG_QT_APP_TOOL_BAR_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_TOOL_BAR_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <map>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class QToolBar;
class QToolButton;
class QIconSet;
class QObject;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppToolBarService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppToolBarService);
public:
    IgQtAppToolBarService (IgState *state, QWidget *mainWindow);
    ~IgQtAppToolBarService (void);

    // FIXME: Provide methods to manage the tool bar.  Synchronise
    // with menu items and the app context service to enable/disable
    // items.
    QToolBar *toolBar (const int id, const std::string &label);
    QToolButton *toolBarButton (IgState *state,
				const int barId,
				const int buttonId,
				const QIconSet &iconSet,
				const std::string &name,
				QObject	*target,
				const char *slot,
				const char *signal = "");
    
     QToolButton * getToolBarButton (const int barId,
				     const int buttonId);
				     
private:
    IgState		*m_state;
    QWidget		*m_mainWindow;

    // undefined semantics
    IgQtAppToolBarService (const IgQtAppToolBarService &);
    IgQtAppToolBarService &operator= (const IgQtAppToolBarService &);
    
    // QUESTION: should we use QWidget instead?
    typedef std::map<int, QToolBar *> 		ToolBarMap;
    typedef std::map<int, QToolButton *> 	ToolButtonMap;
    
    ToolBarMap		m_toolBarMap;
    ToolButtonMap	m_toolButtonMap;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_TOOL_BAR_SERVICE_H
