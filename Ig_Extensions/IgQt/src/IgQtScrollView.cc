//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgQt/interface/IgQtScrollView.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtScrollView::IgQtScrollView (QWidget * parent /*= 0*/,
                                const char * name /*= 0*/,
				WFlags f /*= 0*/)
    : QScrollView (parent, name, f)
{ }

void IgQtScrollView::resizeEvent (QResizeEvent *event)
{
    QScrollBar *sb = horizontalScrollBar ();
    if (sb->value () != sb->minValue ())
        sb->setValue (sb->minValue ());
    sb = verticalScrollBar ();
    if (sb->value () != sb->minValue ())
        sb->setValue (sb->minValue ());
    QScrollView::resizeEvent (event);
}

