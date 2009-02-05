//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/IgQtAxisAlignment.h"
#include "Ig_Extensions/IgIcons/interface/yz_small.xpm"
#include "Ig_Extensions/IgIcons/interface/xz_small.xpm"
#include "Ig_Extensions/IgIcons/interface/yx_small.xpm"
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qwhatsthis.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAxisAlignment::IgQtAxisAlignment (QWidget *parent,
                                      float min /* = 0. */,
				      float max /* = 1. */,
				      const char *name /* = 0 */)
  : m_widget	(new QButtonGroup (parent, name)),
    m_xalign	(new QPushButton ("X", m_widget, name)),
    m_yalign	(new QPushButton ("Y", m_widget, name)),
    m_zalign	(new QPushButton ("Z", m_widget, name)),
    m_alignAxis (2),
    m_min	(min),
    m_max	(max)
{
    m_widget->setFrameShape (QButtonGroup::NoFrame);
    m_widget->setTitle ("");
    
    QHBoxLayout *agroupl = new QHBoxLayout (m_widget, 0, 20);
    m_xalign->setMaximumSize (QSize (30, 30));
    agroupl->addWidget (m_xalign);
    agroupl->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
    m_yalign->setMaximumSize (QSize (30, 30));
    agroupl->addWidget (m_yalign);
    agroupl->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
    m_zalign->setMaximumSize (QSize (30, 30));
    agroupl->addWidget (m_zalign);

    m_xalign->setPixmap (QPixmap (yz_small_xpm));
    m_yalign->setPixmap (QPixmap (xz_small_xpm));
    m_zalign->setPixmap (QPixmap (yx_small_xpm));
    
    QWhatsThis::add (m_xalign, "Align to x-axis");
    QWhatsThis::add (m_yalign, "Align to y-axis");
    QWhatsThis::add (m_zalign, "Align to z-axis");
    
    connect (m_xalign, SIGNAL (clicked ()),
	     this,     SLOT   (setXAlign  ()));
    connect (m_yalign, SIGNAL (clicked ()),
	     this,     SLOT   (setYAlign  ()));
    connect (m_zalign, SIGNAL (clicked ()),
	     this,     SLOT   (setZAlign  ()));
}

QWidget*
IgQtAxisAlignment::widget () const
{ return m_widget; }

float
IgQtAxisAlignment::minAxisAlign (void) const
{ return m_min; }

float
IgQtAxisAlignment::maxAxisAlign (void) const
{ return m_max; }

SbVec3f
IgQtAxisAlignment::value () const
{     
    switch (m_alignAxis)
    {
      case 0:
        return SbVec3f (m_max, m_min, m_min);
      case 1:
        return SbVec3f (m_min, m_max, m_min);
      default:
        return SbVec3f (m_min, m_min, m_max);
    }
}

////////////////////////////////////////////////////////////////////
bool
IgQtAxisAlignment::xAlign (void) const
{ return m_alignAxis == 0; }

bool
IgQtAxisAlignment::yAlign (void) const
{ return m_alignAxis == 1; }

bool
IgQtAxisAlignment::zAlign (void) const
{ return m_alignAxis == 2; }

////////////////////////////////////////////////////////////////////
void
IgQtAxisAlignment::setMinAxisAlign (float newval)
{ m_min = newval; }

void
IgQtAxisAlignment::setMaxAxisAlign (float newval)
{ m_max = newval; }

////////////////////////////////////////////////////////////////////
void
IgQtAxisAlignment::setAlign (unsigned short axis)
{
    switch (axis)
    {
      case 0:
        setXAlign ();
	break;
      case 1:
         setYAlign ();
	break;
      case 2:
         setZAlign ();
	break;
      default:
        ASSERT (false);
    }
}

void
IgQtAxisAlignment::setXAlign (void)
{
    m_alignAxis = 0; 
    setDirty ();
}

void
IgQtAxisAlignment::setYAlign (void)
{
    m_alignAxis = 1;
    setDirty ();
}

void
IgQtAxisAlignment::setZAlign (void)
{
    m_alignAxis = 2;
    setDirty ();
}
