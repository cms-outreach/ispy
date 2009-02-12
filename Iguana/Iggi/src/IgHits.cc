//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/interface/IgHits.h"
#include <QPainter>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgHits::IgHits (const std::vector<QPointF>& points, IggiItem* parent /*= 0*/)
    : IggiItem (parent),
      m_points (points),
      m_color (Qt::lightGray)
{
    m_boundingRect = calcBoundingRect ();
}

IgHits::~IgHits (void)
{}

QRectF 
IgHits::boundingRect (void) const
{ 
    return m_boundingRect;
}

QRectF
IgHits::calcBoundingRect (void) const
{
    // this method builds rectangles with the given points and unites them to
    // one big rectangle. This might exclude sometimes some small parts of the
    // curve, since the curves are not limited to the convex hull.
    QRectF rec;
    unsigned n = m_points.size ();
	
    if (n > 1)
    {
	rec.setTopLeft (m_points [0]);
	rec.setBottomRight (m_points [1]);
	QPointF lastUsed = m_points [1];
		
	if (n > 2)
	{
	    for (unsigned i = 2; i < m_points.size (); i++)
	    {
		if (! rec.contains (m_points [i]))
		{
		    QRectF temp (m_points [i], lastUsed);
		    rec = rec.united (temp);
		    lastUsed = m_points [i];
		}
	    }
	}
    }
    rec = rec.normalized ();
	
    // give the rectange a bit of a brim
    float brim = 5.0;
    rec.setTop ((rec.top () > 0.) ? (rec.top () + brim) : (rec.top () - brim));
    rec.setBottom ((rec.bottom () > 0.) ? (rec.bottom () + brim) : (rec.bottom () - brim));
    rec.setLeft ((rec.left () > 0.) ? (rec.left () + brim) : (rec.left () - brim));
    rec.setRight ((rec.right () > 0.) ? (rec.right () + brim) : (rec.right () - brim));

    return rec;
}

void 
IgHits::drawPoints (QPainter* painter, const QPen& pen)
{
    painter->setPen (pen);

    const float circle = 0.4;
    for (unsigned i = 0; i < m_points.size (); i++)
    {
	const float x_corner = m_points [i].x () - circle / 2.;
	const float y_corner = m_points [i].y () - circle / 2.;
	painter->drawEllipse (QRectF (x_corner, y_corner, circle, circle));
    }
}

void
IgHits::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    bool showBoundingBox = false;
    painter->setPen (pen ());
    
    drawPoints (painter, QPen (m_color));
    if (showBoundingBox)
	painter->drawRect (calcBoundingRect ());
}
