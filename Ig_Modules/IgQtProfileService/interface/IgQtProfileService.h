#ifndef __IG_QT_PROFILE_SERVICE_H_
#define __IG_QT_PROFILE_SERVICE_H_

#include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
#include "Ig_Modules/IgQtBrowser/interface/config.h"

class IgQtProfile;
class QMenuBar;
class QWidget;
class QStatusBar;
class IgOProfMainWindow;
class IgOProfConfiguration;
/**This class creates a profile GUI widget, given a widget in which to embed, a menu bar and a status bar to use.
 */
class IG_QT_BROWSER_API IgQtProfileService : public IgStateElement
{
public:
    IG_DECLARE_STATE_ELEMENT (IgQtProfileService);
    IgQtProfileService (IgState *state, QMenuBar *menuBar, QWidget *parent,QStatusBar *statusBar, bool inDialog);
    ~IgQtProfileService (void);
private:
    IgState *m_state;
    QMenuBar *m_menuBar;
    QWidget *m_parent;
    QStatusBar *m_statusBar;
    
    IgOProfConfiguration *m_config;
    IgOProfMainWindow	*m_profiler;

    // undefined semantics
    IgQtProfileService (const IgQtProfileService &);
    IgQtProfileService &operator= (const IgQtProfileService &);
};
#endif
