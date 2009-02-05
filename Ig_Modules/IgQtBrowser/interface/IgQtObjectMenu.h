#ifndef IG_QT_BROWSER_IG_QT_OBJECT_MENU_H
# define IG_QT_BROWSER_IG_QT_OBJECT_MENU_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include <qkeysequence.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QPopupMenu;
class QString;
class QObject;
class QKeySequence;
class QIconSet;
class QPixmap;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Popup menu for object-specific actions.

    This is an interface highly similar to #QPopupMenu, except it
    requires item ids to be provided and automatically sorts the
    underlying menu to the id order.  It also provides the means to
    record which items are relevant, i.e. should actually appear on
    the menu.

    There is only one menu for the #IgQtObjectMenuService used for all
    pop-up contexts regardless of the type of object it is applied to.
    (If the menu is torn off, Qt will have two copies of it, but all
    the actions such enabling and disabling the items still works the
    same and most clients should not care.)  Observers of
    #IgQtObjectMenuMessage should add their items to this object upon
    receiving a creation message for an object that is relevant to
    them.  Upon receiving an update menu message, they should update
    the items with #setItemRelevant, #setItemEnabled, #setItemChecked,
    and so forth.  Setting an item not relevant removes it from the
    menu and destroys the underlying object.  Always use subMenu() and
    hasItem() to determine if the menu has the desired item or not.

    The caller should register Qt slots with each item it creates,
    otherwise nothing will happen on item selection.  Each item must
    have an explicitly defined id (hence an interface different from
    #QPopupMenu).  The menu items will be sorted according to the ids
    in order to avoid the visual appearance changing according to the
    observer invocation order.

    This class is only a proxy through which items can be inserted
    into the menu; the rest of the menu logic is elsewhere (mainly in
    #IgQtObjectMenuService).  */

class IG_QT_BROWSER_API IgQtObjectMenu
{
public:
    struct ItemDef
    {
	int		subid;		//< Optional sub-menu id
	int		id;		//< Menu item id
	const char	*name;		//< Menu item text
	const char	*member;	//< Slot
	int		param;		//< Action parameter
    };

    IgQtObjectMenu (QPopupMenu *menu);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor


    // Bulk operations
    void			insertFromDefs (const QObject *receiver,
						const ItemDef *defs,
						unsigned n);
    void			removeFromDefs (const ItemDef *defs,
						unsigned n);

    // Individual operations
    IgQtObjectMenu *		subMenu (int id) const;
    bool			hasItem (int id) const;
    void			removeItem (int id);

    IgQtObjectMenu *		insertSubMenu (int id, const QString &text);
    IgQtObjectMenu *		insertSubMenu (int id, const QIconSet &icon,
					       const QString &text);
    IgQtObjectMenu *		insertSubMenu (int id, const QPixmap &pixmap);
    IgQtObjectMenu *		insertSubMenu (int id, const QIconSet &icon,
					       const QPixmap &pixmap);

    int				insertItem (int id, const QString &text);
    int				insertItem (int id, const QIconSet &icon,
					    const QString &text);
    int				insertItem (int id, const QPixmap &pixmap);
    int				insertItem (int id, const QIconSet &icon,
					    const QPixmap &pixmap);

    int				insertItem (int id,
					    const QString &text,
					    const QObject *receiver,
					    const char *member,
					    const QKeySequence &accel = 0);
    int				insertItem (int id,
					    const QIconSet &icon,
					    const QString &text,
					    const QObject *receiver,
					    const char *member,
					    const QKeySequence &accel = 0);
    int				insertItem (int id,
					    const QPixmap &pixmap,
					    const QObject *receiver,
					    const char *member,
					    const QKeySequence &accel = 0);
    int				insertItem (int id,
					    const QIconSet &icon,
					    const QPixmap &pixmap,
					    const QObject *receiver,
					    const char *member,
					    const QKeySequence &accel = 0);

    // int			insertItem (int id, QWidget *widget);
    // int			insertItem (int id, const QIconSet &icon,
    //					    QCustomMenuItem *custom);
    // int			insertItem (int id, QCustomMenuItem *custom);

    int				insertSeparator (int id);

    QKeySequence		accel (int id) const;
    QIconSet *			iconSet (int id) const;
    QString			text (int id) const;
    QPixmap *			pixmap (int id) const;
    QString			whatsThis (int id) const;
    int				itemParameter (int id) const;

    void			setAccel (int id, const QKeySequence &key);
    void			changeItem (int id, const QString &text);
    void			changeItem (int id, const QPixmap &pixmap);
    void			changeItem (int id, const QIconSet &icon, const QString &text);
    void			changeItem (int id, const QIconSet &icon, const QPixmap &pixmap);
    void			setWhatsThis (int id, const QString &text);
    bool			connectItem (int id, const QObject *receiver, const char *member);
    bool			disconnectItem (int id, const QObject *receiver, const char *member);
    bool			setItemParameter (int id, int param);

    bool			isItemActive (int id) const;
    bool			isItemEnabled (int id) const;
    bool			isItemChecked (int id) const;

    void			setItemEnabled (int id, bool enable);
    void			setItemChecked (int id, bool check);

private:
    unsigned			itemPosition (int id) const;
    unsigned			subMenuPosition (int id, bool exact) const;
    void			clearDummy (void);
    void			setDummy (void);

    typedef std::vector<IgQtObjectMenu *>	SubMenus;

    QPopupMenu *		m_menu;
    SubMenus			m_submenus;
    int				m_dummy;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_OBJECT_MENU_H
