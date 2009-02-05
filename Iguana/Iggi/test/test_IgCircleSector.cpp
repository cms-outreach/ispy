//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/test/test_main.h"
#include "Iguana/Iggi/interface/IgCircleSector.h"
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
	IgCircleSector* sec1 = new IgCircleSector (10, 40, 120, 0, center);
	
// 	center.setX (30);
// 	center.setY (0);
	IgCircleSector* sec2 = new IgCircleSector (50, 90, 120, 120, center);
	
// 	center.setX (60);
// 	center.setY (0);
	IgCircleSector* sec3 = new IgCircleSector (100, 150, 120, 240, center);
	
	group->addToGroup (sec1);
	group->addToGroup (sec2);
	group->addToGroup (sec3);
    return group;
}
