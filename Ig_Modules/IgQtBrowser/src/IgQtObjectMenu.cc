//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenu.h"
#include <qpopupmenu.h>
#undef emit
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define INSERT_SUB_1(t1,a1)						\
  IgQtObjectMenu *IgQtObjectMenu::insertSubMenu (int id, const t1 &a1)	\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (! m_menu->findItem (id));					\
									\
      clearDummy ();							\
									\
      QPopupMenu	*subpopup = new QPopupMenu (m_menu);		\
      IgQtObjectMenu	*submenu = new IgQtObjectMenu (subpopup);	\
      unsigned		subpos = subMenuPosition (id, false);		\
      unsigned		pos = itemPosition (id);			\
      m_submenus.insert (m_submenus.begin () + subpos, submenu);	\
      m_menu->insertItem (a1, subpopup, id, pos);			\
      return submenu;							\
  }

#define INSERT_SUB_2(t1,a1,t2,a2)					\
  IgQtObjectMenu *IgQtObjectMenu::insertSubMenu (int id, const t1 &a1,	\
						 const t2 &a2)		\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (! m_menu->findItem (id));					\
									\
      clearDummy ();							\
									\
      QPopupMenu	*subpopup = new QPopupMenu (m_menu);		\
      IgQtObjectMenu	*submenu = new IgQtObjectMenu (subpopup);	\
      unsigned		subpos = subMenuPosition (id, false);		\
      unsigned		pos = itemPosition (id);			\
      m_submenus.insert (m_submenus.begin () + subpos, submenu);	\
      m_menu->insertItem (a1, a2, subpopup, id, pos);			\
      return submenu;							\
  }

//////////////////////////////////////////////////////////////////////
#define INSERT_SIMPLE_1(t1,a1)						\
  int IgQtObjectMenu::insertItem (int id, const t1 &a1)			\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (! m_menu->findItem (id));					\
      clearDummy ();							\
									\
      unsigned pos = itemPosition (id);					\
      VERIFY (m_menu->insertItem (a1, id, pos) == id);			\
      return id;							\
  }
#define INSERT_FULL_1(t1,a1)						\
  int IgQtObjectMenu::insertItem (int id, const t1 &a1,			\
				  const QObject *receiver,		\
				  const char *member,			\
				  const QKeySequence &accel)		\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (! m_menu->findItem (id));					\
      clearDummy ();							\
									\
      unsigned pos = itemPosition (id);					\
      VERIFY (m_menu->insertItem (a1, receiver, member, accel, id, pos) == id);\
      return id;							\
  }

//////////////////////////////////////////////////////////////////////
#define INSERT_SIMPLE_2(t1,a1,t2,a2)					\
  int IgQtObjectMenu::insertItem (int id, const t1 &a1, const t2 &a2)	\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (! m_menu->findItem (id));					\
      clearDummy ();							\
									\
      unsigned pos = itemPosition (id);					\
      VERIFY (m_menu->insertItem (a1, a2, id, pos) == id);		\
      return id;							\
  }
#define INSERT_FULL_2(t1,a1,t2,a2)					\
  int IgQtObjectMenu::insertItem (int id, const t1 &a1, const t2 &a2,	\
				  const QObject *receiver,		\
				  const char *member,			\
				  const QKeySequence &accel)		\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (! m_menu->findItem (id));					\
      clearDummy ();							\
									\
      unsigned pos = itemPosition (id);					\
      VERIFY (m_menu->insertItem (a1, a2, receiver, member, accel, id, pos) == id);\
      return id;							\
  }

//////////////////////////////////////////////////////////////////////
#define SHADOW_GET(get,t)						\
  t IgQtObjectMenu::get (int id) const					\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (m_menu->findItem (id));					\
      return m_menu->get (id);						\
  }

#define SHADOW_SET_1(set,t1,a1)						\
  void IgQtObjectMenu::set (int id, const t1 &a1)			\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (m_menu->findItem (id));					\
      m_menu->set (id, a1);						\
  }

#define SHADOW_SET_2(set,t1,a1,t2,a2)					\
  void IgQtObjectMenu::set (int id, const t1 &a1, const t2 &a2)		\
  {									\
      ASSERT (m_menu);							\
      ASSERT (m_submenus.size () <= m_menu->count ());			\
      ASSERT (m_menu->findItem (id));					\
      m_menu->set (id, a1, a2);						\
  }

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtObjectMenu::IgQtObjectMenu (QPopupMenu *menu)
    : m_menu (menu),
      m_dummy (-1)
{
    // FIXME: We know there might be a tear-off.  Try not to.
    ASSERT (m_menu && m_menu->count () <= 1);
    setDummy ();
}

//////////////////////////////////////////////////////////////////////
unsigned
IgQtObjectMenu::itemPosition (int id) const
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());

    for (unsigned i = 0; i < m_menu->count (); ++i)
	if (m_menu->idAt (i) >= id)
	    return i;

    return m_menu->count ();
}

unsigned
IgQtObjectMenu::subMenuPosition (int id, bool exact) const
{
    // If @a exact, must find an item here, not just insert position;
    // if not found, return m_submenu.size ().
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());

    for (unsigned i = 0, j = 0; i < m_menu->count (); ++i)
    {
	int		itemId = m_menu->idAt (i);
	QMenuItem	*item = m_menu->findItem (itemId);

	ASSERT (item);
	ASSERT (j <= m_submenus.size ());
	ASSERT (! item->popup () || j < m_submenus.size ());
	ASSERT (! item->popup () || m_submenus [j]->m_menu == item->popup ());

	if (itemId == id || (itemId > id && ! exact))
	    return j;
	if (itemId > id && exact)
	    break;

	if (item->popup ())
	    ++j;
    }

    return m_submenus.size ();
}

//////////////////////////////////////////////////////////////////////
IgQtObjectMenu *
IgQtObjectMenu::subMenu (int id) const
{
    unsigned pos = subMenuPosition (id, true);
    if (pos < m_submenus.size ())
	return m_submenus [pos];
    else
	return 0;
}

INSERT_SUB_1	(QString, text)
INSERT_SUB_2	(QIconSet, icon, QString, text)

INSERT_SUB_1	(QPixmap, pixmap)
INSERT_SUB_2	(QIconSet, icon, QPixmap, pixmap)

INSERT_SIMPLE_1 (QString, text)
INSERT_FULL_1   (QString, text)
INSERT_SIMPLE_2 (QIconSet, icon, QString, text)
INSERT_FULL_2   (QIconSet, icon, QString, text)

INSERT_SIMPLE_1 (QPixmap, pixmap)
INSERT_FULL_1   (QPixmap, pixmap)
INSERT_SIMPLE_2 (QIconSet, icon, QPixmap, pixmap)
INSERT_FULL_2   (QIconSet, icon, QPixmap, pixmap)

int
IgQtObjectMenu::insertSeparator (int id)
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (! m_menu->findItem (id));
    clearDummy ();
    unsigned pos = itemPosition (id);
    m_menu->insertSeparator (pos);
    m_menu->setId (pos, id);
    return id;
}

SHADOW_GET   (accel,         QKeySequence)
SHADOW_SET_1 (setAccel,      QKeySequence, key)

SHADOW_GET   (iconSet,       QIconSet *)

SHADOW_GET   (text,          QString)
SHADOW_SET_1 (changeItem,    QString, text)
SHADOW_SET_2 (changeItem,    QIconSet, icon, QString, text)

SHADOW_GET   (pixmap,        QPixmap *)
SHADOW_SET_1 (changeItem,    QPixmap, pixmap)
SHADOW_SET_2 (changeItem,    QIconSet, icon, QPixmap, pixmap)

SHADOW_GET   (whatsThis,     QString)
SHADOW_SET_1 (setWhatsThis,  QString, text)

SHADOW_GET   (itemParameter, int)

bool
IgQtObjectMenu::connectItem (int id, const QObject *receiver,
			     const char *member)
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    return m_menu->connectItem (id, receiver, member);
}

bool
IgQtObjectMenu::disconnectItem (int id, const QObject *receiver,
				const char *member)
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    return m_menu->disconnectItem (id, receiver, member);
}

bool
IgQtObjectMenu::setItemParameter (int id, int param)
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    return m_menu->setItemParameter (id, param);
}

//////////////////////////////////////////////////////////////////////
bool
IgQtObjectMenu::isItemActive (int id) const
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    return m_menu->isItemActive (id);
}

bool
IgQtObjectMenu::isItemEnabled (int id) const
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    return m_menu->isItemEnabled (id);
}

bool
IgQtObjectMenu::isItemChecked (int id) const
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    return m_menu->isItemChecked (id);
}

void
IgQtObjectMenu::setItemEnabled (int id, bool enable)
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    m_menu->setItemEnabled (id, enable);
}

void
IgQtObjectMenu::setItemChecked (int id, bool check)
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    ASSERT (m_menu->findItem (id));
    m_menu->setItemChecked (id, check);
}

//////////////////////////////////////////////////////////////////////
bool
IgQtObjectMenu::hasItem (int id) const
{
    ASSERT (m_menu);
    ASSERT (m_submenus.size () <= m_menu->count ());
    return m_menu->findItem (id) != 0;
}

void
IgQtObjectMenu::removeItem (int id)
{
    if (hasItem (id))
    {
	ASSERT (m_menu->findItem (id));
	if (m_menu->findItem (id)->popup ())
	{
	    unsigned pos = subMenuPosition (id, true);
	    if (pos < m_submenus.size ())
		m_submenus.erase (m_submenus.begin () + pos);
	}
	m_menu->removeItem (id);
    }
    setDummy ();
}

//////////////////////////////////////////////////////////////////////
void
IgQtObjectMenu::clearDummy (void)
{
    if (m_dummy != -1)
    {
	m_menu->removeItem (m_dummy);
	m_dummy = -1;
    }
}

void
IgQtObjectMenu::setDummy (void)
{
    // If the menu has become empty and it is not the top level one,
    // add a simple note to help the user understand the situation.
    // FIXME: We check for one, not zero, because of the tear off
    // handle.  Sort this out -- we shouldn't know if it has one or
    // not.
    if (m_menu->count () == 0 && ! dynamic_cast<QPopupMenu *> (m_menu->parent ()))
    {
	ASSERT (m_dummy == -1);
	m_dummy = m_menu->insertItem ("(Nothing applicable)");
	m_menu->setItemEnabled (m_dummy, false);
    }
}

//////////////////////////////////////////////////////////////////////
void
IgQtObjectMenu::insertFromDefs (const QObject *receiver,
				const ItemDef *defs,
				unsigned n)
{
    ASSERT (receiver);
    ASSERT (! n || defs);

    for (unsigned i = 0; i < n; ++i)
    {
	if (defs [i].subid == -1 && ! defs [i].member && defs [i].name)
	{
	    // Insert a sub-menu as such if it doesn't exist yet
	    if (! subMenu (defs [i].id))
		insertSubMenu (defs [i].id, defs [i].name);
	}
	else
	{
	    // If the item has sub-menu owner, fetch it
	    IgQtObjectMenu *menu = this;
	    if (defs [i].subid != -1)
		VERIFY (menu = subMenu (defs [i].subid));

	    // If the item doesn't exist yet, add it
	    if (! menu->hasItem (defs [i].id))
		if (defs [i].name)
		    menu->setItemParameter (menu->insertItem (defs [i].id,
							      defs [i].name,
							      receiver,
							      defs [i].member),
					    defs [i].param);
		else
		    menu->insertSeparator (defs [i].id);
	}
    }
}

void
IgQtObjectMenu::removeFromDefs (const ItemDef *defs, unsigned n)
{
    ASSERT (! n || defs);

    IgQtObjectMenu *menu;
    for (unsigned i = 0; i < n; ++i)
    {
	if (defs [i].subid == -1 && ! defs [i].member)
	{
	    // If it is an empty sub-menu, remove it
	    if ((menu = subMenu (defs [i].id)) && ! menu->m_menu->count ())
		removeItem (defs [i].id);
	}
	else
	{
	    // If the item has sub-menu owner, fetch it
	    IgQtObjectMenu *menu = this;
	    if (defs [i].subid != -1)
		menu = subMenu (defs [i].subid);

	    // Remove the item if the menu is still there
	    if (menu)
	    {
		menu->removeItem (defs [i].id);

		// If a sub-menu became empty, remove it
		if (menu != this && ! menu->m_menu->count ())
		    removeItem (defs [i].subid);
	    }
	}
    }
}
