//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/interface/IggiItem.h"
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

IggiItem::IggiItem (QAbstractGraphicsShapeItem* parent) 
    : QAbstractGraphicsShapeItem (parent) 
{
    this->setFlag (QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents (true);
}

void
IggiItem::hoverEnterEvent (QGraphicsSceneHoverEvent* event)
{
    QPen pen = this->pen ();
    pen.setColor (Qt::red);
    pen.setWidth (3);
    setPen (pen);
    this->scene ()->update ();
}

void
IggiItem::hoverLeaveEvent (QGraphicsSceneHoverEvent* event)
{    
    QPen pen = this->pen ();
    pen.setColor (Qt::blue);
    pen.setWidth (1);
    setPen (pen);
    this->scene ()->update ();
}

void
IggiItem::mousePressEvent (QGraphicsSceneMouseEvent* event)
{    
    QPen pen = this->pen ();
    pen.setColor (Qt::green);
    pen.setWidth (2);
    setPen (pen);
    this->scene ()->update ();
}
