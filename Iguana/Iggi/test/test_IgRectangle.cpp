//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/test/test_main.h"
#include "Iguana/Iggi/interface/IgRectangle.h"
#include <QPointF>
#include <QGraphicsItem>

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
	
	QPointF center (0,0);
	IgRectangle* rect1 = new IgRectangle (50, 50, center);
	
	center.setX (30);
	center.setY (0);
	IgRectangle* rect2 = new IgRectangle (130, 100, center);
	
	center.setX (60);
	center.setY (0);
	IgRectangle* rect3 = new IgRectangle (210, 200, center);
	
	group->addToGroup (rect1);
	group->addToGroup (rect2);
	group->addToGroup (rect3);
    return group;
}
