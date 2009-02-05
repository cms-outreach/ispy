#ifndef IG_QT_BROWSER_IG_QT_APP_DEBUG_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_DEBUG_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgQtAppDebug;
class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppDebugService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppDebugService);
public:
    static const int	MENU_ID_DEBUG		= 9000;
    static const int	MENU_ID_DEBUG_RESOURCES	= 9001;
    static const int	MENU_ID_DEBUG_LIBRARIES	= 9002;
    static const int	MENU_ID_DEBUG_PLUGINS	= 9003;
    static const int	MENU_ID_DEBUG_DUMPSTATE	= 9004;
    static const int	MENU_ID_DEBUG_SESSION	= 9005;
    static const int	MENU_ID_DEBUG_POOLS	= 9006;
    static const int	MENU_ID_DEBUG_CRASH	= 9007;
    static const int	MENU_ID_DEBUG_DEBUGME	= 9008;
    static const int	MENU_ID_DEBUG_PROFILER_SEP = 9100;
    static const int	MENU_ID_DEBUG_PROFILER	= 9101;

    IgQtAppDebugService (IgState *state, QWidget *mainWindow);
    ~IgQtAppDebugService (void);

    // FIXME: Add menu item to show various debugging information.
    // Use ResourceInfo::info and TimeInfo::process_times.  Show
    // currently loaded services.

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;
    IgQtAppDebug		*m_info;

    // undefined semantics
    IgQtAppDebugService (const IgQtAppDebugService &);
    IgQtAppDebugService &operator= (const IgQtAppDebugService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_DEBUG_SERVICE_H
