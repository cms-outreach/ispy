#ifndef IG_QT_BROWSER_IG_QT_APP_HELP_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_HELP_SERVICE_H

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

class IgQtAppHelp;
class QWidget;

class IG_QT_BROWSER_API IgQtAppHelpService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppHelpService);
public:
    static const int	MENU_ID_HELP		= 10000;
    static const int	MENU_ID_HELP_IGUANA_WEB	= 10001;
    static const int	MENU_ID_HELP_REPORT_BUG = 10002;
    static const int	MENU_ID_HELP_WHATSTHIS	= 10003;
    static const int	MENU_ID_HELP_BUTTONS	= 10004;
    static const int	MENU_ID_HELP_ABOUT_QT	= 10009;
    static const int	MENU_ID_HELP_ABOUT	= 10010;

    IgQtAppHelpService (IgState *state, QWidget *mainWindow);
    ~IgQtAppHelpService (void);

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;
    IgQtAppHelp			*m_about;
};

#endif // IG_QT_BROWSER_IG_QT_APP_HELP_SERVICE_H
