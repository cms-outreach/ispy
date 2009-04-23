//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyItem.h"
#include <QPen>
#include <QGraphicsScene>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyItem::ISpyItem (QAbstractGraphicsShapeItem* parent) 
    : QAbstractGraphicsShapeItem (parent) 
{
    this->setFlag (QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents (true);
}

void
ISpyItem::hoverEnterEvent (QGraphicsSceneHoverEvent* event)
{
    QPen pen = this->pen ();
    pen.setColor (Qt::red);
    pen.setWidth (3);
    setPen (pen);
    this->scene ()->update ();
}

void
ISpyItem::hoverLeaveEvent (QGraphicsSceneHoverEvent* event)
{    
    QPen pen = this->pen ();
    pen.setColor (Qt::blue);
    pen.setWidth (1);
    setPen (pen);
    this->scene ()->update ();
}

void
ISpyItem::mousePressEvent (QGraphicsSceneMouseEvent* event)
{    
    QPen pen = this->pen ();
    pen.setColor (Qt::green);
    pen.setWidth (2);
    setPen (pen);
    this->scene ()->update ();
}
