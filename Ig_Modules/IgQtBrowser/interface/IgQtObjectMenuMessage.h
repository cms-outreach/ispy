#ifndef IG_QT_BROWSER_IG_QT_OBJECT_MENU_MESSAGE_H
# define IG_QT_BROWSER_IG_QT_OBJECT_MENU_MESSAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepresentable;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A message notifying that a context-sensitive menu for an object
    is about to be popped up or needs to be updated.

    The message's #IgRepresentable points to the object for which menu
    items should be provided; it will never be null.  Observers should
    always check with the menu()'s IgQtObjectMenu::seen() whether
    their items are already listed in the menu, as this message can be
    sent with the items already added, e.g. by a browser to update a
    torn-off menu for a new selection focus.  On the other hand,
    already added items might have been destroyed since the previous
    message.

    Items on the menu should be updated to appropriate states before
    returning: enabled, checked and relevant.  Newly added items are
    always relevant but should be updated for the other states.
    (FIXME: explain relevant vs. enabled policy.)  (FIXME: multiple
    selection?)  */
class IG_QT_BROWSER_API IgQtObjectMenuMessage
{
public:
    IgQtObjectMenuMessage (IgQtObjectMenu *menu, IgRepresentable *object);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    IgQtObjectMenu *		menu (void) const;
    IgRepresentable *		object (void) const;

private:
    IgQtObjectMenu		*m_menu;	//< The menu frontend.
    IgRepresentable		*m_object;	//< The selected object.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/// Construct a new message for a menu and context object.
inline
IgQtObjectMenuMessage::IgQtObjectMenuMessage (IgQtObjectMenu *menu,
					      IgRepresentable *object)
    : m_menu (menu),
      m_object (object)
{}

/// Return the menu.
inline IgQtObjectMenu *
IgQtObjectMenuMessage::menu (void) const
{ return m_menu; }

/// Return the context object.
inline IgRepresentable *
IgQtObjectMenuMessage::object (void) const
{ return m_object; }

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_OBJECT_MENU_MESSAGE_H
