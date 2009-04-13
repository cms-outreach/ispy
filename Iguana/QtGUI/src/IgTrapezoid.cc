//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/QtGUI/interface/IgTrapezoid.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTrapezoid::IgTrapezoid (float upperSide, float lowerSide, float height, const QPointF& center /*= QPointF (0., 0.)*/, ISpyItem* parent /*= 0*/) :
	ISpyItem (parent),
	m_center (center),
	m_height (height),
	m_upper  (upperSide),
	m_lower  (lowerSide)
{}

IgTrapezoid::~IgTrapezoid (void)
{}

void 
IgTrapezoid::setHeight (float value)
{
	if (value > 0.)
	{
		m_height = value;
	}
	else
	{
		std::cerr << "Given value should be > 0! You provided: " << value << std::endl;
	}
}

void 
IgTrapezoid::setUpperSide (float value)
{
	if (value > 0.)
	{
		m_upper = value;
	}
	else
	{
		std::cerr << "Given value should be > 0! You provided: " << value << std::endl;
	}
}

void 
IgTrapezoid::setLowerSide (float value)
{
	if (value > 0.)
	{
		m_lower = value;
	}
	else
	{
		std::cerr << "Given value should be > 0! You provided: " << value << std::endl;
	}
}

void 
IgTrapezoid::setCenter (const QPointF& value)
{
	m_center = value;
}

QRectF 
IgTrapezoid::boundingRect (void) const
{
	const float longSide = (m_upper > m_lower) ? (m_upper) : (m_lower);
	return QRectF (m_center.x () - longSide/2., m_center.y () - m_height/2., longSide, m_height);
}

void
IgTrapezoid::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	const float x = m_center.x ();
	const float y = m_center.y ();
	
	const QPointF points[4] = {
		QPointF (x - m_upper/2., y - m_height/2.),
		QPointF (x + m_upper/2., y - m_height/2.),
		QPointF (x + m_lower/2., y + m_height/2.),
		QPointF (x - m_lower/2., y + m_height/2.)
	};
	
	painter->drawConvexPolygon (points, 4);
}
