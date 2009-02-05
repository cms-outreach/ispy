
//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtTreeBrowser/interface/IgQtTreeRep.h"
#include "Ig_Modules/IgQtTreeBrowser/interface/IgQtTreeBrowser.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include <qapplication.h>
#include <qpainter.h>
#include <qpixmap.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

// FIXME: For some reason GCC 3.2 wants these instantiated
#ifdef __GNUC__
const unsigned int   IgQtTreeRep::TRAVERSE;
const unsigned int   IgQtTreeRep::VISIBLE;
#endif // __GNUC__

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtTreeRep *
IgQtTreeRep::create (IgQtTreeBrowser *in,
		     IgQtTreeRep *parent,
		     IgQtTreeRep *before,
		     QString name, bool traverse,
		     bool visible, bool opened)
{
    if (parent && before)
	return new IgQtTreeRep (parent, before, name, traverse, visible, opened);
    else if (parent)
	return new IgQtTreeRep (parent, name, traverse, visible, opened);
    else if (before)
	return new IgQtTreeRep (in, before, name, traverse, visible, opened);
    else
	return new IgQtTreeRep (in, name, traverse, visible, opened);
}

////////////////////////////////////////////////////////////////////////////////
IgQtTreeRep::IgQtTreeRep (IgQtTreeRep *parent, IgQtTreeRep *before,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (parent, before, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

IgQtTreeRep::IgQtTreeRep (IgQtTreeRep *parent,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (parent, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

IgQtTreeRep::IgQtTreeRep (IgQtTreeBrowser *in, IgQtTreeRep *before,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (in, before, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

IgQtTreeRep::IgQtTreeRep (IgQtTreeBrowser *in,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (in, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

void
IgQtTreeRep::init (bool traverse, bool visible, bool opened)
{
    bold (! parent ());


    setCheck (TRAVERSE_FIELD, traverse, false);
    setCheck (VISIBLE_FIELD,  visible, false);
    setExpandable (true);
    // Delay until we get a context
    flag (AUTO_OPEN, opened);
}

IgQtTreeRep::~IgQtTreeRep (void)
{
    // The destruction may come upon me in two ways: the object is
    // going away and the context is deleting me, or someone else
    // killed me (e.g. Qt).  In the former case the caller will have
    // done the right thing of getting rid of references to me.
    // However if my context *was* deleted, I must not delete it
    // again; if so, it has already set its rep to null, which we use
    // as a clue.

    ASSERT (m_context);
    if (m_context->rep ())
    {
	ASSERT (m_context->rep () == this);
	m_context->erase (false);
	delete m_context;
    }
}

void
IgQtTreeRep::context (IgRepContext *context)
{
    ASSERT (! m_context);
    ASSERT (context);
    m_context = context;

    // OK, it is now safe to open up this one
    if (flag (AUTO_OPEN))
	setOpen (true);
}

//////////////////////////////////////////////////////////////////////
IgRepContext *
IgQtTreeRep::context (void) const
{ return m_context; }

IgModel *
IgQtTreeRep::model (void) const
{ return static_cast<IgQtTreeBrowser *> (listView ()); }

void
IgQtTreeRep::setOpen (bool opened)
{
    if (opened && ! flag (EXPANDED))
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	IgBrowserMethods::expand (m_context->object (), this);
	QApplication::restoreOverrideCursor ();
	setExpandable (childCount ());
    }

    if (opened != flag (EXPANDED))
	flag (EXPANDED, opened);

    QListViewItem::setOpen (opened);
    IgBrowserMethods::commit (this, m_context->object (), CASCADE_FIELD);
}

void
IgQtTreeRep::paintCell (QPainter *painter, const QColorGroup &cg,
			int column, int width, int align)
{
    QFont fold (painter->font ());
    QFont fnew (painter->font ());

    fnew.setBold (bold ());
    fnew.setItalic (italic ());

    painter->setFont (fnew);
    QListViewItem::paintCell (painter, columnColor (column, cg),
			      column, width, align);
    painter->setFont (fold);
}

int
IgQtTreeRep::width (const QFontMetrics &, const QListView *lv, int c) const
{
    QFont fnew (lv->font ());
    fnew.setBold (bold ());
    fnew.setItalic (italic ());
    QFontMetrics fm (fnew);
    return QListViewItem::width (fm, lv, c);
}

//////////////////////////////////////////////////////////////////////
bool
IgQtTreeRep::traversed (void) const
{
    IgQtTreeRep *p = static_cast<IgQtTreeRep *> (parent ());
    for (; p; p = static_cast<IgQtTreeRep *> (p->parent ()))
	if (! p->flag (TRAVERSE))
	    return false;

    return true;
}
    

void
IgQtTreeRep::repaintChildren (QListViewItem *item)
{
    item->repaint ();
    for (item = item->firstChild (); item; item = item->nextSibling ())
	repaintChildren (item);
}

QColorGroup
IgQtTreeRep::columnColor (int column, const QColorGroup &cg) const
{
    QColorGroup cgnew (cg);
    if (column == 0 && !traversed ())
	cgnew.setColor (QColorGroup::Text, cg.mid ());
    return cgnew;
}

void
IgQtTreeRep::setCheck (int column, bool value, bool /* known */)
{
    QListViewItem *item;
    switch (column)
    {
    case TRAVERSE_FIELD:
        flag (TRAVERSE, value);

        // Update children to reflect visibility status
        for (item = firstChild (); item; item = item->nextSibling ())
            repaintChildren (item);
        break;

    case VISIBLE_FIELD:
        flag (VISIBLE, value);
        break;

    default:
        ASSERT (false);
    }
}
