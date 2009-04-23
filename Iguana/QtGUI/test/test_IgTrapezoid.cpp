//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/test/test_main.h"
#include "Iguana/QtGUI/interface/IgTrapezoid.h"
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
	IgTrapezoid* rect1 = new IgTrapezoid (50, 100, 50, center);
	
	center.setX (30);
	center.setY (0);
	IgTrapezoid* rect2 = new IgTrapezoid (150, 200, 100, center);
	
	center.setX (60);
	center.setY (0);
	IgTrapezoid* rect3 = new IgTrapezoid (200, 300, 150, center);
	
	group->addToGroup (rect1);
	group->addToGroup (rect2);
	group->addToGroup (rect3);
    return group;
}
