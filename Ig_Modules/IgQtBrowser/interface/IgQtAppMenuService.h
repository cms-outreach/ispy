#ifndef IG_QT_BROWSER_IG_QT_APP_MENU_SERVICE_H
# define IG_QT_BROWSER_IG_QT_APP_MENU_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class QMainWindow;
class QMenuBar;
class QPopupMenu;
class QMenuData;
class QString;
class QObject;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppMenuService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppMenuService);
public:
    static const int	MENU_GROUP_RANGE = 1000;
    static const int	MENU_SUBGROUP_RANGE = 50;

    static const int	MENU_ID_FILE 		= 0;
    static const int	MENU_ID_FILE_SAVE	= 1;
    static const int	MENU_ID_FILE_PRINT	= 2;
    static const int	MENU_ID_FILE_QUIT 	= (MENU_ID_FILE
						   + 10 * MENU_SUBGROUP_RANGE);
    static const int	MENU_ID_HELP 		= 100 * MENU_GROUP_RANGE;

    IgQtAppMenuService (IgState *state, QWidget *mainWindow);
    ~IgQtAppMenuService (void);

    // FIXME: Add methods to manage the main menu (and popups?).
    // Organise menus by ids given in insertion stuff.  Use the app
    // context service to manage the enabling of menu items on popup.
    // Insert separators if ids change by certain boundary (1000?).

    QMenuBar *			menuBar (void);
    QPopupMenu *		subMenu (int id);
    QPopupMenu *		subMenu (int id1, int id2);
    QPopupMenu *		findByName (QMenuData *menu, 
					    const char *itemName);
    
    static int			index (QMenuData *menu, int id);
    static int 			isIdThere (QMenuData *menu, int id);
    static void			insertItem (QMenuData *parentMenu,
					    const QString &text,
					    QObject *target,
					    const char *slot,
					    int id,
					    IgState *state = 0);
private:
    IgState			*m_state;
    QMainWindow			*m_mainWindow;

    // undefined semantics
    IgQtAppMenuService (const IgQtAppMenuService &);
    IgQtAppMenuService &operator= (const IgQtAppMenuService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_MENU_SERVICE_H
