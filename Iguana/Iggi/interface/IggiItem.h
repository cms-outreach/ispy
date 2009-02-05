#ifndef IGGI_IGGI_ITEM_H
# define IGGI_IGGI_ITEM_H

//<<<<<< INCLUDES                                                       >>>>>>

//# include <QGraphicsItem>
# include <QAbstractGraphicsShapeItem>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiItem : public QAbstractGraphicsShapeItem
{

public:
    IggiItem (QAbstractGraphicsShapeItem* parent = 0);
    virtual ~IggiItem () {};

    virtual void 	zoomIn (void) {} /*= 0*/;
    virtual void 	zoomOut (void) {} /*= 0*/;

protected:
    
    virtual void	hoverEnterEvent (QGraphicsSceneHoverEvent* event);
    virtual void	hoverLeaveEvent (QGraphicsSceneHoverEvent* event);
    virtual void 	mousePressEvent (QGraphicsSceneMouseEvent* event);
    
//     QRectF boundingRect() const
// 	{
// 	    qreal penWidth = 1;
// 	    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
// 			  20 + penWidth / 2, 20 + penWidth / 2);
// 	}
// 
//     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
// 	       QWidget *widget)
// 	{
// 	    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
// 	}
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_ITEM_H
