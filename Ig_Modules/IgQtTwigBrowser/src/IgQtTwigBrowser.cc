//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtTwigBrowser/interface/IgQtTwigBrowser.h"
#include "Ig_Modules/IgQtTwigBrowser/interface/IgQtTwigRep.h"
#include "Ig_Modules/IgQtTwigBrowser/interface/xtypeinfo.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionMessage.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"
#include <qheader.h>
#include <qevent.h>
#include <qlist.h>
#include <qpopupmenu.h>
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgQtTwigBrowser::catalogLabel (void)
{ return "Twig"; }

IgQtTwigBrowser::IgQtTwigBrowser (IgState *state, 
				  IgSite *site)
    : QListView (IgQtSite::hostFrom (site)),
      m_state (state),
      m_discardRelease (false)
{
    ASSERT (IgSelectionService::get (m_state));
    ASSERT (IgQtObjectMenuService::get (m_state));
    IgQtSite::host (site, this);
    IgSelectionService::get (m_state)
	->add (lat::CreateCallback (this, &IgQtTwigBrowser::selectMessage));

    //header ()->setResizeEnabled (false);
    header ()->setMovingEnabled (false);

    // FIXME: enable sorting, but default to off!
    setSorting (-1, false);

    addColumn ("Object", visibleWidth () - 20);
    addColumn ("Visibility", 20);
    setVScrollBarMode (QScrollView::AlwaysOn);
    resize (200, height ());
    
    // addColumn ("Type");
    setRootIsDecorated (true);
    setMultiSelection (false);
    connect (this,SIGNAL(selectionChanged()), this,SLOT(onSelectionChange()));
    connect (this,SIGNAL(selectionChanged()), this,SLOT(slotItemChanged()));
    connect (this,SIGNAL(contextMenuRequested(QListViewItem *,
					      const QPoint &, int)),
	     this,SLOT(contextMenu(QListViewItem *, const QPoint &, int)));

    show ();
}

IgQtTwigBrowser::~IgQtTwigBrowser (void)
{
    IgSelectionService::get (m_state)
	->remove (lat::CreateCallback (this, &IgQtTwigBrowser::selectMessage));
}


void
IgQtTwigBrowser::browse (IgRepresentable *object)
{
    // FIXME: This was stateless, no rep created, need to change that.
    // FIXME: broadcast selection message
    /* IgRepContext *context =*/ IgRepSet::lookup (object, this, true);
    // FIXME: model change event?
    // FIXME: select?
    // if (context)
    //     IgSelectionService::get (m_state)
    //		->broadcast (IgSelectionMessage (context));
}

void
IgQtTwigBrowser::select (IgQtTwigRep *rep)
{
    // FIXME: protect against looping from signals?
    if (! rep)
    {
	QList<IgQtTwigRep> *selected = getSelectedItems ();
	for (IgQtTwigRep *rep = selected->first(); rep; rep = selected->next())
	    setSelected (rep, false);
	delete selected;
    }
    else
    {
	if (! isSelected (rep))
	    setSelected (rep, true);

	ensureItemVisible (rep);
    }
}

//////////////////////////////////////////////////////////////////////
void
IgQtTwigBrowser::selectMessage (IgSelectionMessage message)
{
    // FIXME: avoid re-emitting the signal?
    if (! message.context ())
	select (0);
    else if (IgQtTwigRep *rep = dynamic_cast<IgQtTwigRep *>
	     (IgRepSet::lookup (message.context (), this, true)))
	// FIXME: update display
	select (rep);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#if 0
//! Activated whenever the visibility toggler is pushed.
void
IgQtTwigBrowser::checkSelected (void)
{
    // FIXME: gets called for saving
    QList<IgQtTwigRep> *selected = getSelectedItems ();
    for (IgQtTwigRep *rep = selected->first (); rep; rep = selected->next())
    {
	// this will automatically set the visibility, emit the signals
	// broadcast a change message and update all other browsers
	rep->check (true);
    }

    delete selected;
}

//! Activated whenever the visibility toggler is pushed.
void
IgQtTwigBrowser::updateTree (void)
{
    // FIXME: gets called after opening a file
    // FIXME: this should be handled in the application data logic!
    QList<IgQtTwigRep> *selected = getCheckedItems (IgQtTwigRep::VISIBLE_FIELD);
    for (IgQtTwigRep *rep = selected->first (); rep; rep = selected->next ())
    {
	// FIXME: by definition all the items *are* already checked...

	// this will automatically set the visibility, emit the signals
	// broadcast a change message and update all other browsers
	rep->check (rep->checked ());
    }

    // FIXME: broadcast some message that will dump out the info?
    // printTrees ();
    delete selected;
}

//! Print out the status of all the twigs.
//  void
//  IgQtTwigBrowser::printTrees (void)
//  {
//      for (List_of_Twigs::iterator i = rootTwigs.begin ();
//           i != rootTwigs.end(); i++)
//          (*i)->printTree();
//  }
#endif // 0

//! Register changes in the selection list and broadcasts it on the bus.
void
IgQtTwigBrowser::onSelectionChange (void)
{
    // FIXME: broadcast a single message for multiple / single selection?
    QList<IgQtTwigRep> *selected = getSelectedItems ();
    for (IgQtTwigRep *rep = selected->first (); rep; rep = selected->next ())
	IgSelectionService::get (m_state)
	    ->broadcast (IgSelectionMessage (rep->context ()));
    delete selected;
}

void
IgQtTwigBrowser::resizeEvent (QResizeEvent *event)
{
  QListView::resizeEvent (event);
  
  int diff = event->size().width () - event->oldSize().width ();
  if (diff != 0)
  {
    diff = 0;
    for (int i = 1; i < columns (); i++)
      diff += columnWidth (i);
    setColumnWidth (0, visibleWidth () - diff);
    updateContents ();
  }
}

void
IgQtTwigBrowser::contentsMouseDoubleClickEvent (QMouseEvent *event)
{
    ASSERT (event);

    QPoint		pos = contentsToViewport (event->pos ());
    QListViewItem	*item = itemAt (pos);
    IgQtTwigRep		*rep = static_cast<IgQtTwigRep *> (item);

    ASSERT (!item || dynamic_cast<IgQtTwigRep *> (item));

    if (rep && rep->magic ())
	// Prevent the item from opening on double click
	/* emit */ doubleClicked (rep);
    else
	QListView::contentsMouseDoubleClickEvent (event);
}

void
IgQtTwigBrowser::contentsMousePressEvent (QMouseEvent *event)
{
    ASSERT (event);

    QPoint		pos = contentsToViewport (event->pos ());
    QListViewItem	*item = itemAt (pos);
    int			column = isInsideChecker (pos.x ());

    if (item && column != -1)
    {
	// Ask the rep to make the change (and commit the change back
	// to the object, and broadcast it to all other browsers).
	ASSERT (dynamic_cast<IgQtTwigRep *> (item));
	IgQtTwigRep *rep = static_cast<IgQtTwigRep *> (item);
	rep->check (column, ! rep->checked (column));
	m_discardRelease = true;
    }
    else
    {
	m_discardRelease = false;
	QListView::contentsMousePressEvent (event);
    }
}

void
IgQtTwigBrowser::contentsMouseReleaseEvent (QMouseEvent *event)
{
    if (m_discardRelease)
	m_discardRelease = false;
    else
	QListView::contentsMouseReleaseEvent (event);
}

int
IgQtTwigBrowser::isInsideChecker (int x)
{
    // Readjust for horizontally scrolled view.
    x += contentsX ();

    // We have two checkable columns: traversal (1) and visibility
    // (2).  Check if the position `x' is in either; and return the
    // index of that column if so; otherwise return -1.
    static const int	CHECK_COLUMN = IgQtTwigRep::VISIBLE_FIELD;
    int			i;

    for (i = 0; i < CHECK_COLUMN; ++i)
	if ((x -= columnWidth (i)) < 0)
	    return -1;

    if ((x -= columnWidth (i)) < 0)
	return i;

    return -1;
}

//////////////////////////////////////////////////////////////////////
void
IgQtTwigBrowser::contextMenu (QListViewItem *item, const QPoint &pos, int)
{
    ASSERT (!item || dynamic_cast<IgQtTwigRep *> (item));
    if (! item)
	return;

    IgQtObjectMenuService *svc = IgQtObjectMenuService::get (m_state);
    IgRepresentable	  *object = static_cast<IgQtTwigRep *> (item)
				    ->context ()->object ();

    // Pop up the menu and update it in case it was torn off.
    svc->request (object)->exec (pos);
    svc->postUpdate (object);
}

//////////////////////////////////////////////////////////////////////
//! Replacement for AdvListViewItemIterator
QListViewItem *
IgQtTwigBrowser::getNextUpItem (QListViewItem *item)
{
    if (QListViewItem *parent = item->parent ())
	if (QListViewItem *next = parent->nextSibling ())
	    return next;
	else
	    return getNextUpItem (parent);

    return 0;
}

//! Replacement for AdvListViewItemIterator
QListViewItem *
IgQtTwigBrowser::getNextItem (QListViewItem *item)
{
    if (QListViewItem *next = item->firstChild ())
	return next;
    else if ((next = item->nextSibling ()))
	return next;
    else
	return getNextUpItem (item);
}

//! Walk the tree to find the reps that are selected
QList<IgQtTwigRep> *
IgQtTwigBrowser::getSelectedItems (void)
{
    QList<IgQtTwigRep> *selected = new QList<IgQtTwigRep>;
    selected->setAutoDelete (false);

    for (QListViewItem *item = firstChild (); item; item = getNextItem (item))
        if (item->isSelected ())
	{
	    ASSERT (dynamic_cast<IgQtTwigRep *> (item));
	    IgQtTwigRep *rep = static_cast<IgQtTwigRep *> (item);
            selected->append (rep);
	}

    return selected;
}

//////////////////////////////////////////////////////////////////////
//! Walk the tree to find the reps that are checked
QList<IgQtTwigRep> *
IgQtTwigBrowser::getCheckedItems (int column)
{
    QList<IgQtTwigRep> *checked = new QList<IgQtTwigRep>;
    checked->setAutoDelete (false);

    for (QListViewItem *item = firstChild (); item; item = getNextItem (item))
        if (static_cast<IgQtTwigRep *> (item)->checked (column))
	{
	    ASSERT (dynamic_cast<IgQtTwigRep *> (item));
	    IgQtTwigRep *rep = static_cast<IgQtTwigRep *> (item);
            checked->append (rep);
	}

    return checked;
}

//////////////////////////////////////////////////////////////////////
void
IgQtTwigBrowser::slotItemChanged (void)
{
    // LOG(..., "slot item changed" << std::endl);
}

void
IgQtTwigBrowser::slotItemRMClicked (void)
{
    // LOG(..., "slot item right mouse clicked" << std::endl);
}
