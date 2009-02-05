//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DZoomControl.h"
#include "Ig_Extensions/IgQt/interface/IgQtPixButton.h"
#include <classlib/utils/DebugAids.h>
#include <qlayout.h>
#include <qcombobox.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DZoomControl::Ig3DZoomControl (QWidget *parent, const char *name)
    : QWidget (parent, name),
      m_zoomValue (2)
{
    QHBoxLayout *layout = new QHBoxLayout (this, 0, 5);
    QComboBox	*items = new QComboBox (this);

    // items->setFixedHeight (20);
    // items->setFixedWidth (50);
    items->insertItem ("2");
    items->insertItem ("5");
    items->insertItem ("10");
    items->insertItem ("100");
    items->setCurrentItem (0);
    items->setEditable (false);
    // items->setSizePolicy (QSizePolicy (QSizePolicy::Minimum,
    // QSizePolicy::Minimum));

    IgQtPixButton *in = new IgQtPixButton (QPixmap (), this);
    // in->setFixedHeight (20);
    // in->setFixedWidth (60);
    in->setText ("Zoom In");
    // in->setSizePolicy (QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));

    IgQtPixButton *out = new IgQtPixButton (QPixmap (), this);
    // out->setFixedHeight (20);
    // out->setFixedWidth (60);
    out->setText ("Zoom Out");
    // out->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));

    layout->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
    layout->addWidget (in);
    layout->addWidget (items);
    layout->addWidget (out);
    layout->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));

    connect (in,    SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect (out,   SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect (items, SIGNAL(activated(const QString &)),
	     this,  SLOT(setZoom(const QString &)));
}

void
Ig3DZoomControl::zoomIn (void)
{ emit zoom (1.0/m_zoomValue); }

void
Ig3DZoomControl::zoomOut (void)
{ emit zoom (m_zoomValue); }

unsigned int
Ig3DZoomControl::zoomValue (void)
{ return m_zoomValue; }

void
Ig3DZoomControl::setZoom (const QString &value)
{ m_zoomValue = value.toUInt (); }
