//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtTreeBrowser/interface/IgQtTreeBrowser.h"
#include "Ig_Modules/IgQtTreeBrowser/interface/IgQtTreeRep.h"
#include "Ig_Modules/IgQtTreeBrowser/interface/xtypeinfo.h"
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
IgQtTreeBrowser::catalogLabel (void)
{ return "Tree"; }

IgQtTreeBrowser::IgQtTreeBrowser (IgState *state, 
				  IgSite *site)
    : QListView (IgQtSite::hostFrom (site)),
      m_state (state),
      m_discardRelease (false)
{
    ASSERT (IgSelectionService::get (m_state));
    ASSERT (IgQtObjectMenuService::get (m_state));
    IgQtSite::host (site, this);
    IgSelectionService::get (m_state)
	->add (lat::CreateCallback (this, &IgQtTreeBrowser::selectMessage));

    //header ()->setResizeEnabled (false);
    header ()->setMovingEnabled (false);

    // FIXME: enable sorting, but default to off!
    setSorting (-1, false);

    addColumn ("Object", visibleWidth () - 20);
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

    this->header ()->hide ();
    show ();
}

IgQtTreeBrowser::~IgQtTreeBrowser (void)
{
    IgSelectionService::get (m_state)
	->remove (lat::CreateCallback (this, &IgQtTreeBrowser::selectMessage));
}


void
IgQtTreeBrowser::browse (IgRepresentable *object)
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
IgQtTreeBrowser::select (IgQtTreeRep *rep)
{
    // FIXME: protect against looping from signals?
    if (! rep)
    {
	QList<IgQtTreeRep> *selected = getSelectedItems ();
	for (IgQtTreeRep *rep = selected->first(); rep; rep = selected->next())
	    setSelected (rep, false);
	delete selected;
    }
    else
    {
	// Select the item and make sure it is cascaded out and visible
	for (QListViewItem *item = rep->parent (); item; item = item->parent())
	    if (! item->isOpen ())
		item->setOpen (true);

	if (! isSelected (rep))
	    setSelected (rep, true);

	ensureItemVisible (rep);
    }
}

//////////////////////////////////////////////////////////////////////
void
IgQtTreeBrowser::selectMessage (IgSelectionMessage message)
{
    // FIXME: avoid re-emitting the signal?
    if (! message.context ())
	select (0);
    else if (IgQtTreeRep *rep = dynamic_cast<IgQtTreeRep *>
	     (IgRepSet::lookup (message.context (), this, true)))
	// FIXME: update display
	select (rep);
}

//! Register changes in the selection list and broadcasts it on the bus.
void
IgQtTreeBrowser::onSelectionChange (void)
{
    // FIXME: broadcast a single message for multiple / single selection?
    QList<IgQtTreeRep> *selected = getSelectedItems ();
    for (IgQtTreeRep *rep = selected->first (); rep; rep = selected->next ())
	IgSelectionService::get (m_state)
	    ->broadcast (IgSelectionMessage (rep->context ()));
}

void
IgQtTreeBrowser::resizeEvent (QResizeEvent *event)
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
IgQtTreeBrowser::contentsMouseDoubleClickEvent (QMouseEvent *event)
{
    ASSERT (event);

    QPoint		pos = contentsToViewport (event->pos ());
    QListViewItem	*item = itemAt (pos);
    IgQtTreeRep		*rep = static_cast<IgQtTreeRep *> (item);

    ASSERT (!item || dynamic_cast<IgQtTreeRep *> (item));

    if (rep && rep->magic ())
	// Prevent the item from opening on double click
	/* emit */ doubleClicked (rep);
    else
	QListView::contentsMouseDoubleClickEvent (event);
}

void
IgQtTreeBrowser::contentsMousePressEvent (QMouseEvent *event)
{
    ASSERT (event);

    m_discardRelease = false;
    QListView::contentsMousePressEvent (event);
}

void
IgQtTreeBrowser::contentsMouseReleaseEvent (QMouseEvent *event)
{
    if (m_discardRelease)
	m_discardRelease = false;
    else
	QListView::contentsMouseReleaseEvent (event);
}

//////////////////////////////////////////////////////////////////////
void
IgQtTreeBrowser::contextMenu (QListViewItem *item, const QPoint &pos, int)
{
    ASSERT (!item || dynamic_cast<IgQtTreeRep *> (item));
    if (! item)
	return;

    IgQtObjectMenuService *svc = IgQtObjectMenuService::get (m_state);
    IgRepresentable	  *object = static_cast<IgQtTreeRep *> (item)
				    ->context ()->object ();

    // Pop up the menu and update it in case it was torn off.
    svc->request (object)->exec (pos);
    svc->postUpdate (object);
}

//////////////////////////////////////////////////////////////////////
//! Replacement for AdvListViewItemIterator
QListViewItem *
IgQtTreeBrowser::getNextUpItem (QListViewItem *item)
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
IgQtTreeBrowser::getNextItem (QListViewItem *item)
{
    if (QListViewItem *next = item->firstChild ())
	return next;
    else if ((next = item->nextSibling ()))
	return next;
    else
	return getNextUpItem (item);
}

//! Walk the tree to find the reps that are selected
QList<IgQtTreeRep> *
IgQtTreeBrowser::getSelectedItems (void)
{
    QList<IgQtTreeRep> *selected = new QList<IgQtTreeRep>;
    selected->setAutoDelete (false);

    for (QListViewItem *item = firstChild (); item; item = getNextItem (item))
        if (item->isSelected ())
	{
	    ASSERT (dynamic_cast<IgQtTreeRep *> (item));
	    IgQtTreeRep *rep = static_cast<IgQtTreeRep *> (item);
            selected->append (rep);
	}

    return selected;
}

//////////////////////////////////////////////////////////////////////
void
IgQtTreeBrowser::slotItemChanged (void)
{
    // LOG(..., "slot item changed" << std::endl);
}

void
IgQtTreeBrowser::slotItemRMClicked (void)
{
    // LOG(..., "slot item right mouse clicked" << std::endl);
}
