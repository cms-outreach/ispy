//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgQt/interface/IgQtPixButton.h"
#include <qevent.h>
#include <qpopupmenu.h>
#include <qwhatsthis.h>
#include <qcursor.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtPixButton::IgQtPixButton (QWidget *parent, const char *name)
    : QToolButton (parent, name)
{ initialise (QPixmap ()); }

IgQtPixButton::IgQtPixButton (QPixmap pixmap, QWidget *parent,const char *name)
    : QToolButton (parent, name)
{ initialise (pixmap); }

void
IgQtPixButton::initialise (QPixmap pixmap)
{
    setAutoRepeat (true);
    setAutoRaise (true);
    if (!pixmap.isNull ())
      setPixmap (pixmap);
}

void
IgQtPixButton::mousePressEvent (QMouseEvent * event)
{
    QPopupMenu *menu = popup ();
    if (event->button () == Qt::RightButton && menu)
    {
      QString text (QWhatsThis::textFor(this, QPoint (), true));
      unsigned int count = menu->count ();
      if(!text.isNull())
      {
        menu->insertSeparator ();
        menu->insertItem ("&What's This?", this, SLOT (showWhatsThisHelp (void)));
      }
      popup ()->exec (QCursor::pos ());
      event->accept ();
      while (menu->count () > count)
        menu->removeItemAt (count);
    }
    else    
     QToolButton::mousePressEvent (event);
}

void
IgQtPixButton::showWhatsThisHelp (void)
{ QWhatsThis::display (QWhatsThis::textFor(this, QPoint (), true)); }
