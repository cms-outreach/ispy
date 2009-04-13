//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/test/test_main.h"
#include "Iguana/QtGUI/interface/IgTrack.h"
#include <QGraphicsLineItem>
#include <QPointF>
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
    std::vector<QPointF> tangents;
	
    // HERMITE examples
	
    // lying 's'
    // 	points.push_back (QPointF (-100,0));
    // 	points.push_back (QPointF (100,0));
    // 	tangents.push_back (QPointF (0,100));
    // 	tangents.push_back (QPointF (0,100));
	
    // circular thing
    // 		double f = 2.;
    // 	points.push_back (QPointF (0,-100));
    // 	points.push_back (QPointF (-100,0));
    // 	points.push_back (QPointF (0,100));
    // 	points.push_back (QPointF (50,0));
    // 	points.push_back (QPointF (0,-80));
    // 	tangents.push_back (f*QPointF (-100,0));
    // 	tangents.push_back (f*QPointF (0,100));
    // 	tangents.push_back (f*QPointF (100,0));
    // 	tangents.push_back (f*QPointF (0,-100));
    // 	tangents.push_back (f*QPointF (-100,0));

    // 	draw a spiral
    double a = 30.;
    double b = 0.2;
    for (double t = 0.1; t < 10.; t += 0.5)
    {
	double x_t  = a * exp (b*t) * cos (t);
	double y_t  = a * exp (b*t) * sin (t);
	double x_tt = a * exp (b*t) * (b*cos(t) - sin(t));
	double y_tt = a * exp (b*t) * (b*sin(t) + cos(t));
	points.push_back   (QPointF (x_t, y_t));
	tangents.push_back (QPointF (x_tt, y_tt));
    }

    // only SPLINE examples
		
    // draw an 's'
    // 	points.push_back (QPointF (   0,-100));
    // 	points.push_back (QPointF ( 100, 0));
    // 	points.push_back (QPointF (   0, 100));
    // 	points.push_back (QPointF (-100, 200));
    // 	points.push_back (QPointF (   0, 300));

    // draw a circle
    // 	points.push_back (QPointF (-100, -100));
    // 	points.push_back (QPointF ( 100, -100));
    // 	points.push_back (QPointF ( 100, 100));
    // 	points.push_back (QPointF (-100, 100));
    // 	points.push_back (QPointF (-100, -100));

    // draw a stylistic 'm'
    // 	points.push_back (QPointF (-100,  -100));
    // 	points.push_back (QPointF (-80, 50));
    // 	points.push_back (QPointF (-50, -50));
    // 	points.push_back (QPointF (-10, -70));
    // 	points.push_back (QPointF (0, 0));
    // 	points.push_back (QPointF (15, -50));
    // 	points.push_back (QPointF (50, 50));
    // 	points.push_back (QPointF (150, 100));
	
    // 	IgTrack* track = new IgTrack (points, tangents, IgTrack::HERMITE);
    IgTrack* track = new IgTrack (points, tangents, IgTrack::SPLINE);
    // 	IgTrack* track = new IgTrack (points, tangents, IgTrack::ALL);

    QPen pen;
    pen.setBrush (Qt::blue);
    pen.setWidth (3);
    track->setPen (pen);
	
    return track;
}
