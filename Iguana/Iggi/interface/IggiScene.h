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
class IggiItem;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiScene : public QGraphicsScene
{
    Q_OBJECT
public:
    IggiScene (QObject *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IggiScene (const QRectF& sceneRect, QObject* parent = 0);
    IggiScene (qreal x, qreal y, qreal width, qreal height, QObject* parent = 0);
    void 		setEventGroup (QGraphicsItemGroup* group);
    QGraphicsItemGroup*	eventGroup (void);

signals:
    void 		itemSelected (IggiItem *selectedItem, const QPointF &itemPosition);

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
