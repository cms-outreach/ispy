//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/test/test_main.h"
#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

QGraphicsItem*
makeTest (void)
{
    QGraphicsItemGroup* group = new QGraphicsItemGroup;

    QGraphicsPathItem* line = new QGraphicsPathItem;
    QPainterPath path;
    path.cubicTo (10, 0, 60, 50, 99, 99);

    line->setPath (path);

//     QGraphicsEllipseItem* point1 = new QGraphicsEllipseItem;
//     point1->setRect (10, 0, 2, 2);
    
//     QGraphicsEllipseItem* point2 = new QGraphicsEllipseItem;
//     point2->setRect (60, 50, 2, 2);

//     QGraphicsEllipseItem* point3 = new QGraphicsEllipseItem;
//     point3->setRect (99, 99, 2, 2);

    QGraphicsPathItem* curve = new QGraphicsPathItem;
    QPainterPath curvePath;
    curvePath.quadTo (99, 0, 99, 99);
    curve->setPath (curvePath);

//     QGraphicsPathItem* curve1 = new QGraphicsPathItem;
//     QPainterPath curvePath1;
//     curvePath1.quadTo (0, 100, 100, 100);
//     curvePath1.quadTo (200, 100, 200, 0);
//     curvePath1.quadTo (200, -100, 300, -100);
//     curvePath1.quadTo (300, -100, 400, 0);
//     curve1->setPath (curvePath1);
    
    group->addToGroup (line);
//     group->addToGroup (point1);
//     group->addToGroup (point2);
//     group->addToGroup (point3);
    group->addToGroup (curve);
//     group->addToGroup (curve1);

    return group;
}
