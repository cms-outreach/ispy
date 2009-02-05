//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgControlBase.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include <classlib/utils/DebugAids.h>
#include <qhbox.h>
#include <qlayout.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgControlBase::IgControlBase (IgControlCategory *parent)
    : m_parent (parent),
      m_tracking (false),
      m_shown (false)
{
    ASSERT (parent);

    connect (this,   SIGNAL(dirty()),        parent, SLOT(setDirty()));
    connect (this,   SIGNAL(clean()),        parent, SLOT(setClean()));
    connect (parent, SIGNAL(tracking(bool)), this,   SLOT(track(bool)));
    connect (parent, SIGNAL(editable(bool)), this,   SLOT(setEditable(bool)));
}

void
IgControlBase::attach (void)
{ connectSignal (); }

void
IgControlBase::detach (void)
{ disconnectSignal (); }

void
IgControlBase::update (void)
{
    if (hasValue ())
    {
	disconnectSignal ();
	updateValue ();
	setDirty (false);
	connectSignal ();
    }
}

void
IgControlBase::apply (void)
{
    // We deliberately do not check for isDirty() here.  The signal
    // should be sent only if either a) user clicked "Apply" button on
    // the control centre dialog and the control category passes it to
    // us, or b) I am tracking changes and became dirty.  Either way
    // we should just simply make the change.
    //
    // This causes a problem if the underlying object changes without
    // the GUI's knowledge, and then the user clicks "Apply".  Should
    // we or should we not apply the settings from the GUI?  On the
    // one hand, if the user didn't change the GUI values (= I am not
    // dirty), we shouldn't change the object.  On the other hand, we
    // should make the object be what is on the dialog.  The correct
    // fix is not to let the GUI get out of synch with the object.
    if (hasValue ())
    {
	detach ();
	applyValue ();
	setDirty (false);
	attach ();
    }
}

void
IgControlBase::track (bool doit)
{
    m_tracking = doit;
    if (m_tracking){
      connectSignal ();
      if (isDirty())
        emit dirty ();
    }
    else
      disconnectSignal ();
}

bool
IgControlBase::hasValue (void) const
{ return true; }

void
IgControlBase::connectSignal (void)
{
    if (hasValue () && m_tracking)
	connect (this,SIGNAL(dirty()), this,SLOT(apply()));
}

void
IgControlBase::disconnectSignal (void)
{
    disconnect (this,SIGNAL(dirty()), this,SLOT(apply()));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QWidget *
IgControlBase::makeIndentedLabel (QString label, QWidget *area)
{
    ASSERT (m_parent);
    return m_parent->makeIndentedLabel (label, area);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int
IgControlBase::gridRows (void) const
{
    ASSERT (m_parent);
    ASSERT (m_parent->bodyLayout ());

    return m_parent->bodyLayout ()->numRows ();
}

void
IgControlBase::addGridWidget (QWidget *widget, int row, int column)
{
    ASSERT (m_parent);
    ASSERT (m_parent->bodyLayout ());

    m_parent->bodyLayout ()->addWidget (widget, row, column);
    m_objects.append (widget);
}

void
IgControlBase::addGridLayout (QLayout *layout, int row, int column)
{
    ASSERT (m_parent);
    ASSERT (m_parent->bodyLayout ());

    m_parent->bodyLayout ()->addLayout (layout, row, column);
    m_objects.append (layout);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
IgControlBase::isShown (void)
{ return m_shown; }

void
IgControlBase::setShown (bool show)
{
    m_shown = show;
    if (show)
	this->show ();
    else
	this->hide ();
}

void
IgControlBase::showLayout (QLayoutItem *item, bool show)
{
    ASSERT (item);

    QLayoutIterator	i = item->iterator ();
    QLayoutItem		*child;

    while ((child = i.current ()))
    {
	showLayout (child, show);
	++i;
    }

    if (QWidget *w = item->widget ())
	if (show)
	    w->show ();
	else
	    w->hide ();
}

void
IgControlBase::show (void)
{
    for (QObject *obj = m_objects.first (); obj; obj = m_objects.next ())
	if (QWidget *w = dynamic_cast<QWidget *> (obj))
	    w->show ();
	else
	    showLayout (dynamic_cast<QLayout *> (obj), true);
}

void
IgControlBase::hide (void)
{
    for (QObject *obj = m_objects.first (); obj; obj = m_objects.next ())
	if (QWidget *w = dynamic_cast<QWidget *> (obj))
	    w->hide ();
	else
	    showLayout (dynamic_cast<QLayout *> (obj), false);
}
