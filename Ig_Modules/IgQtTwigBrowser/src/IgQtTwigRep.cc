//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtTwigBrowser/interface/IgQtTwigRep.h"
#include "Ig_Modules/IgQtTwigBrowser/interface/IgQtTwigBrowser.h"
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
const unsigned int   IgQtTwigRep::TRAVERSE;
const unsigned int   IgQtTwigRep::VISIBLE;
#endif // __GNUC__

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtTwigRep *
IgQtTwigRep::create (IgQtTwigBrowser *in,
		     IgQtTwigRep *parent,
		     IgQtTwigRep *before,
		     QString name, bool traverse,
		     bool visible, bool opened)
{
    if (parent && before)
	return new IgQtTwigRep (parent, before, name, traverse, visible, opened);
    else if (parent)
	return new IgQtTwigRep (parent, name, traverse, visible, opened);
    else if (before)
	return new IgQtTwigRep (in, before, name, traverse, visible, opened);
    else
	return new IgQtTwigRep (in, name, traverse, visible, opened);
}

////////////////////////////////////////////////////////////////////////////////
IgQtTwigRep::IgQtTwigRep (IgQtTwigRep *parent, IgQtTwigRep *before,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (parent, before, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

IgQtTwigRep::IgQtTwigRep (IgQtTwigRep *parent,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (parent, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

IgQtTwigRep::IgQtTwigRep (IgQtTwigBrowser *in, IgQtTwigRep *before,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (in, before, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

IgQtTwigRep::IgQtTwigRep (IgQtTwigBrowser *in,
			  QString name, bool traverse, bool visible, bool opened)
    : QListViewItem (in, name),
      m_context (0),
      m_flags (0)
{ init (traverse, visible, opened); }

void
IgQtTwigRep::init (bool traverse, bool visible, bool opened)
{
    bold (! parent ());

    setCheck (TRAVERSE_FIELD, traverse, false);
    setCheck (VISIBLE_FIELD,  visible, false);
    setExpandable (true);
    // Delay until we get a context
    flag (AUTO_OPEN, opened);
}

IgQtTwigRep::~IgQtTwigRep (void)
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
IgQtTwigRep::context (IgRepContext *context)
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
IgQtTwigRep::context (void) const
{ return m_context; }

IgModel *
IgQtTwigRep::model (void) const
{ return static_cast<IgQtTwigBrowser *> (listView ()); }

//////////////////////////////////////////////////////////////////////
void
IgQtTwigRep::check (int column, bool value /* = true */)
{
    // Update the list view and commit the change back to the object,
    // but only if we are changing anything.
    if (value != checked (column))
    {
	setCheck (column, value, true);
	QApplication::setOverrideCursor (Qt::waitCursor);
	IgBrowserMethods::commit (this, m_context->object (), column);
	QApplication::restoreOverrideCursor ();
    }
}

void
IgQtTwigRep::setOpen (bool opened)
{
    if (opened && ! flag (EXPANDED))
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	IgBrowserMethods::expand (m_context->object (), this);
	QApplication::restoreOverrideCursor ();
	setExpandable (childCount ());
	setCheck (TRAVERSE_FIELD, flag (TRAVERSE), true);
    }

    if (opened != flag (EXPANDED))
	flag (EXPANDED, opened);

    QListViewItem::setOpen (opened);
    IgBrowserMethods::commit (this, m_context->object (), CASCADE_FIELD);
}

void
IgQtTwigRep::paintCell (QPainter *painter, const QColorGroup &cg,
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
IgQtTwigRep::width (const QFontMetrics &, const QListView *lv, int c) const
{
    QFont fnew (lv->font ());
    fnew.setBold (bold ());
    fnew.setItalic (italic ());
    QFontMetrics fm (fnew);
    return QListViewItem::width (fm, lv, c);
}

//////////////////////////////////////////////////////////////////////
bool
IgQtTwigRep::traversed (void) const
{
    IgQtTwigRep *p = static_cast<IgQtTwigRep *> (parent ());
    for (; p; p = static_cast<IgQtTwigRep *> (p->parent ()))
	if (! p->flag (TRAVERSE))
	    return false;

    return true;
}
    
void
IgQtTwigRep::setCheck (int column, bool value, bool /* known */)
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
	setPixmap (column, pixmapForState (value));
	break;

    default:
	ASSERT (false);
    }
}

void
IgQtTwigRep::repaintChildren (QListViewItem *item)
{
    item->repaint ();
    for (item = item->firstChild (); item; item = item->nextSibling ())
	repaintChildren (item);
}

QColorGroup
IgQtTwigRep::columnColor (int column, const QColorGroup &cg) const
{
    QColorGroup cgnew (cg);
    if (column == 0 && !traversed ())
	cgnew.setColor (QColorGroup::Text, cg.mid ());
    return cgnew;
}

QPixmap
IgQtTwigRep::pixmapForState (unsigned value)
{
    static const char	*unchecked[] = {
	"    16    13        2            1",
	"  c none",
	"x c #000000",
	"x x x x x x x   ",
	"                ",
	"x           x   ",
	"                ",
	"x           x   ",
	"                ",
	"x           x   ",
	"                ",
	"x           x   ",
	"                ",
	"x           x   ",
	"                ",
	"x x x x x x x   "
    };

    static const char	*unknown[] = {
	"    16    13        3            1",
	"  c none",
	"x c #000000",
	"! c #303030",
	"x x x x x x x   ",
	"                ",
	"x ! ! ! ! ! x   ",
	"                ",
	"x ! ! ! ! ! x   ",
	"                ",
	"x ! ! ! ! ! x   ",
	"                ",
	"x ! ! ! ! ! x   ",
	"                ",
	"x ! ! ! ! ! x   ",
	"                ",
	"x x x x x x x   "
    };

    static const char	*checked[] = {
	"    16    13        2            1",
	"  c none",
	"x c #000000",
	"x x x x x x x   ",
	"               x",
	"x            xx ",
	"            x   ",
	"x xx      xx    ",
	" xxxx    xx     ",
	"x xxxx  xx  x   ",
	"   xxxxxx       ",
	"x   xxxxx   x   ",
	"     xxx        ",
	"x     xx    x   ",
	"                ",
	"x x x x x x x   "
    };

    static QPixmap	uncheckedpix (unchecked);
    static QPixmap	unknownpix (unknown);
    static QPixmap	checkedpix (checked);

    switch (value) {
    case 0: return uncheckedpix;
    case 1: return checkedpix;
    case 2: return unknownpix;
    default: ASSERT (false); return unknownpix;
    };
}
