#ifndef IG_COMMON_OPS_IG_TWIG_OPS_H
# define IG_COMMON_OPS_IG_TWIG_OPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgCommonOps/interface/config.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTwig;
class IgQtObjectMenu;
class IgQtObjectMenuMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** General twig operations for the object-specific context menus.
    (FIXME: This belogs elsewhere, e.g. in IgQtTwigBrowser!).  */
class IG_COMMON_OPS_API IgTwigOps : public QObject
{
    Q_OBJECT
public:
    static const int HS_ALL_LEAVES	= -1;
    static const int HS_ALL_CHILDREN	= -2;
    static const int HS_N		= -3;

    static const int MENU_ENABLE	= 1;
    static const int MENU_ENABLE_UP	= 2;

    static const int MENU_SHOW_SEP	= 10;
    static const int MENU_SHOW_THIS	= 11;
    static const int MENU_SHOW_ONLY	= 12;
    static const int MENU_SHOW_LEVEL	= 13;
    static const int MENU_SHOW_LEAVES	= 1300;
    static const int MENU_SHOW_CHILDREN	= 1301;
    static const int MENU_SHOW_1	= 1302;
    static const int MENU_SHOW_2	= 1303;
    static const int MENU_SHOW_3	= 1304;
    static const int MENU_SHOW_N	= 1305;

    static const int MENU_HIDE_SEP	= 50;
    static const int MENU_HIDE_LEVEL	= 51;
    static const int MENU_HIDE_LEAVES	= 5100;
    static const int MENU_HIDE_CHILDREN	= 5101;
    static const int MENU_HIDE_1	= 5102;
    static const int MENU_HIDE_2	= 5103;
    static const int MENU_HIDE_3	= 5104;
    static const int MENU_HIDE_N	= 5105;

    // implicit constructor
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    void		menuAction (IgQtObjectMenuMessage message);
    void		menuUpdate (void);

public slots:
    void		actionEnable (int id);
    void		actionVisible (int id);
    void		actionShow (int n);
    void		actionHide (int n);

private:
    void		doShow (int n, IgTwig *twig);
    void		doHide (int n, IgTwig *twig);

    IgTwig		*m_twig;
    IgQtObjectMenu	*m_menu;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_COMMON_OPS_IG_TWIG_OPS_H
