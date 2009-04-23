#ifndef IGGI_IGGI_SCENE_H
# define IGGI_IGGI_SCENE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QGraphicsScene>
# include <QObject>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QGraphicsSceneMouseEvent;
class QPointF;
class ISpyItem;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    ISpyScene (QObject *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ISpyScene (const QRectF& sceneRect, QObject* parent = 0);
    ISpyScene (qreal x, qreal y, qreal width, qreal height, QObject* parent = 0);
    void 		setEventGroup (QGraphicsItemGroup* group);
    QGraphicsItemGroup*	eventGroup (void);

signals:
    void 		itemSelected (ISpyItem *selectedItem, const QPointF &itemPosition);

protected:
    void 		mousePressEvent (QGraphicsSceneMouseEvent *event);
    void 		mouseReleaseEvent (QGraphicsSceneMouseEvent *event);

private:
    QGraphicsItemGroup* m_eventGroup;
    QGraphicsItem* 	m_selectedItem;
    QPointF        	m_pos;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_SCENE_H
