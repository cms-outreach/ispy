#ifndef IG_QT_BROWSER_IG_QT_OBJECT_MENU_SERVICE_H
# define IG_QT_BROWSER_IG_QT_OBJECT_MENU_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgDispatcher.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepresentable;
class IgQtObjectMenuMessage;
class IgQtObjectMenu;
class QPopupMenu;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A manager for an object-specific #QPopupMenu.

    This service manages a context-sensitive popup menu for browsers.
    There is one menu that is updated to reflect actions relevant to
    the object at hand.  Browsers can use this service to pop up a
    menu when and where they desire; other parts of the application
    can register with this service to add object-specific actions to
    such menus.

    @sa #IgQtObjectMenuMessage, #IgQtObjectMenu.  */

class IG_QT_BROWSER_API IgQtObjectMenuService
    : public IgStateElement, public IgDispatcher<IgQtObjectMenuMessage>
{
    IG_DECLARE_STATE_ELEMENT (IgQtObjectMenuService);
public:
    IgQtObjectMenuService (IgState *state);
    ~IgQtObjectMenuService (void);

    QPopupMenu *		request (IgRepresentable *object);
    void			postUpdate (IgRepresentable *object);
    IgQtObjectMenu *		menu (void) const;
    QPopupMenu *		realMenu (void) const;

private:
    IgState			*m_state;
    QPopupMenu			*m_menu;
    IgQtObjectMenu		*m_proxy;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_OBJECT_MENU_SERVICE_H
