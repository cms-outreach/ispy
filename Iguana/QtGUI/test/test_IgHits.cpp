//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/test/test_main.h"
#include "Iguana/QtGUI/interface/IgHits.h"
#include <vector>
#include <math.h>

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
    std::vector<QPointF> points;

    // 	draw a spiral
    double a = 30.;
    double b = 0.2;
    for (double t = 0.1; t < 10.; t += 0.5)
    {
	double x_t  = a * exp (b*t) * cos (t);
	double y_t  = a * exp (b*t) * sin (t);
	points.push_back   (QPointF (x_t, y_t));
    }

    IgHits* hits = new IgHits (points);
    QPen pen;
    pen.setBrush (Qt::red);
    pen.setWidth (3);
    hits->setPen (pen);
	
    return hits;    
}
