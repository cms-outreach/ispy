//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgCommonOps/interface/IgTwigOps.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenu.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuMessage.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include <qapplication.h>
#include <qinputdialog.h>
#include <climits>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgTwigOps::menuAction (IgQtObjectMenuMessage message)
{
    static const IgQtObjectMenu::ItemDef defs [] = {
	{ -1, MENU_ENABLE, "Enable Children",
	  SLOT(actionEnable(int)), MENU_ENABLE },

	{ -1, MENU_SHOW_SEP,                  0, 0, -1 },
	{ -1, MENU_SHOW_THIS,                  "Show This",
	  SLOT(actionVisible(int)), MENU_SHOW_THIS },
	{ -1, MENU_SHOW_ONLY,                  "Show Only This",
	  SLOT(actionVisible(int)), MENU_SHOW_ONLY },
	{ -1, MENU_SHOW_LEVEL,                 "Show By Level", 0, -1 },
	{ MENU_SHOW_LEVEL, MENU_SHOW_LEAVES,   "&Leaves",
	  SLOT(actionShow(int)), HS_ALL_LEAVES },
	{ MENU_SHOW_LEVEL, MENU_SHOW_CHILDREN, "&Every Level",
	  SLOT(actionShow(int)), HS_ALL_CHILDREN },
	{ MENU_SHOW_LEVEL, MENU_SHOW_1,        "&1 Level Down",
	  SLOT(actionShow(int)), 1 },
	{ MENU_SHOW_LEVEL, MENU_SHOW_2,        "&2 Levels Down",
	  SLOT(actionShow(int)), 2 },
	{ MENU_SHOW_LEVEL, MENU_SHOW_3,        "&3 Levels Down",
	  SLOT(actionShow(int)), 3 },
	{ MENU_SHOW_LEVEL, MENU_SHOW_N,        "&N Levels Down...",
	  SLOT(actionShow(int)), HS_N },

	{ -1, MENU_HIDE_SEP,                   0, 0, -1 },
	{ -1, MENU_HIDE_LEVEL,                 "Hide By Level", 0, -1 },
	{ MENU_HIDE_LEVEL, MENU_HIDE_LEAVES,   "&Leaves",
	  SLOT(actionHide(int)), HS_ALL_LEAVES },
	{ MENU_HIDE_LEVEL, MENU_HIDE_CHILDREN, "&Every Level",
	  SLOT(actionHide(int)), HS_ALL_CHILDREN },
	{ MENU_HIDE_LEVEL, MENU_HIDE_1,        "&1 Level Down",
	  SLOT(actionHide(int)), 1 },
	{ MENU_HIDE_LEVEL, MENU_HIDE_2,        "&2 Levels Down",
	  SLOT(actionHide(int)), 2 },
	{ MENU_HIDE_LEVEL, MENU_HIDE_3,        "&3 Levels Down",
	  SLOT(actionHide(int)), 3 },
	{ MENU_HIDE_LEVEL, MENU_HIDE_N,        "&N Levels Down...",
	  SLOT(actionHide(int)), HS_N },
    };

    static const int	ndefs = sizeof (defs)/sizeof (defs [0]);

    m_menu = message.menu ();
    ASSERT (m_menu);

    if (! (m_twig = dynamic_cast<IgTwig *> (message.object ())))
	// An irrelevant object, remove our items.
	m_menu->removeFromDefs (defs, ndefs);
    else
    {
	// Add our items if they aren't there yet.
	m_menu->insertFromDefs (this, defs, ndefs);
	menuUpdate ();
    }
}

void
IgTwigOps::menuUpdate (void)
{
    ASSERT (m_menu);
    // Update items to reflect the state of this twig.
    m_menu->changeItem (MENU_ENABLE, m_twig->selfTraverse ()
			? "Disable Children" : "Enable Children");
}

void
IgTwigOps::actionEnable (int id)
{
    ASSERT (m_twig);
    QApplication::setOverrideCursor (Qt::waitCursor);
    switch (id)
    {
    case MENU_ENABLE_UP:
	for (IgTwig *t = m_twig->parent (); t; t = t->parent ())
	{
	    if (! t->selfTraverse ())
		t->selfTraverse (true);
	    if (! t->parent ())
		IgRepSet::invalidate (t, IgTwig::FLAGS_MASK|IgTwig::STRUCTURE_MASK);
	}
	break;

    case MENU_ENABLE:
	m_twig->selfTraverse (! m_twig->selfTraverse ());
	IgRepSet::invalidate (m_twig, IgTwig::FLAGS_MASK | IgTwig::STRUCTURE_MASK);
	break;

    default:
	ASSERT (false);
    }
    QApplication::restoreOverrideCursor ();
}

void
IgTwigOps::actionVisible (int id)
{
    ASSERT (m_twig);
    IgTwig *t;

    QApplication::setOverrideCursor (Qt::waitCursor);
    switch (id)
    {
    case MENU_SHOW_THIS:
	m_twig->selfVisible (true);
	actionEnable (MENU_ENABLE_UP);
	// Action already forced update notification.
	break;

    case MENU_SHOW_ONLY:
	// Hide the whole tree then make `m_twig' visible.
	t = m_twig->root ();
	t->selfVisible (false);
	doHide (HS_ALL_CHILDREN, t);
	m_twig->selfVisible (true);
	IgRepSet::invalidate (t, IgTwig::FLAGS_MASK | IgTwig::STRUCTURE_MASK);
	break;

    default:
	ASSERT (false);
    }
    QApplication::restoreOverrideCursor ();
}

void
IgTwigOps::actionShow (int n)
{
    if (n == HS_N)
    {
	bool ok = false;
	n = QInputDialog::getInteger ("IGUANA",
				      "Please enter the number of levels",
				      4, 1, INT_MAX, 1, &ok);
	if (! ok)
	    return;
    }

    QApplication::setOverrideCursor (Qt::waitCursor);
    doShow (n, m_twig);
    actionEnable (MENU_ENABLE_UP);
    QApplication::restoreOverrideCursor ();
}

void
IgTwigOps::actionHide (int n)
{
    if (n == HS_N)
    {
	bool ok = false;
	n = QInputDialog::getInteger ("IGUANA",
				      "Please enter the number of levels",
				      4, 1, INT_MAX, 1, &ok);
	if (! ok)
	    return;
    }

    QApplication::setOverrideCursor (Qt::waitCursor);
    doHide (n, m_twig);
    IgRepSet::invalidate (m_twig, IgTwig::FLAGS_MASK | IgTwig::STRUCTURE_MASK);
    QApplication::restoreOverrideCursor ();
}

void
IgTwigOps::doShow (int n, IgTwig *twig)
{
    ASSERT (twig);
    if (n == 0)
	return;

    twig->selfTraverse (true);
    twig->expand ();
    for (unsigned i = twig->children (); i; --i)
    {
	IgTwig *child = twig->child (i-1);
	doShow (n > 0 ? n-1 : n, child);

	switch (n)
	{
	case HS_ALL_CHILDREN:
	    child->selfVisible (true);
	    break;
	case HS_ALL_LEAVES:
	    child->selfVisible (child->children () == 0);
	    break;
	default:
	    child->selfVisible (n == 1);
	    break;
	}
    }
}

void
IgTwigOps::doHide (int n, IgTwig *twig)
{
    ASSERT (twig);
    if (n == 0)
	return;

    // Twigs are by default is not visible, so no need to expand the tree
    // (FIXME: is this a valid assumption?)
    for (unsigned i = twig->children (); i; --i)
    {
	IgTwig *child = twig->child (i-1);
	doHide (n > 0 ? n-1 : n, child);

	switch (n)
	{
	case HS_ALL_CHILDREN:
	    child->selfVisible (false);
	    break;
	case HS_ALL_LEAVES:
	    if (child->children () == 0)
		child->selfVisible (false);
	    break;
	default:
	    if (n >= 0)
		child->selfVisible (false);
	    break;
	}
    }
}

