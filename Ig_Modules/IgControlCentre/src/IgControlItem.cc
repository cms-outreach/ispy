//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgControlItem.h"
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

IgControlItem::IgControlItem (void)
    : m_editable (true),
      m_dirty (false)
{}

//////////////////////////////////////////////////////////////////////
bool
IgControlItem::isDirty (void) const
{ return m_dirty; }

void
IgControlItem::setDirty (bool state)
{
    if (state && ! m_dirty)
    {
	m_dirty = true;
	emit dirty ();
    }
    else if (! state && m_dirty)
    {
	m_dirty = false;
	emit clean ();
    }
}

void
IgControlItem::setDirty (void)
{ setDirty (true); }

void
IgControlItem::setClean (void)
{ setDirty (false); }

//////////////////////////////////////////////////////////////////////
void
IgControlItem::setEditable (bool value)
{
    m_editable = value;
    emit editable (value);
}

bool
IgControlItem::isEditable (void) const
{ return m_editable; }
//////////////////////////////////////////////////////////////////////
QWidget *
IgControlItem::makeSpacedBox (QWidget *parent, QWidget *content)
{ 
    QWidget *box = makeBox (parent, content);
    box->layout ()->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding));

    return box;
}

QWidget *
IgControlItem::makeBox (QWidget *parent, QWidget *content)
{
    QHBox *box = new QHBox (parent);
    content->reparent (box, QPoint (0, 0), true);

    return box;
}
