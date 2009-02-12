//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/Iggi/interface/IgCircleSector.h"
#include <iostream>
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

IgCircleSector::IgCircleSector (float innerRadius, float outerRadius, float openingAngle, float startAngle /*= 0*/, 
	                            const QPointF& center /*= QPointF (0., 0.)*/, IggiItem* parent  /*= 0*/) :
	IggiItem  (parent),
	m_center  (center),
	m_inner   (innerRadius),
	m_outer   (outerRadius),
	m_opening (openingAngle),
	m_start   (startAngle)
{}

IgCircleSector::~IgCircleSector (void)
{}

void 
IgCircleSector::setInnerRadius (float value)
{
	if (value >= 0.)
	{
		m_inner = value;
	}
	else
	{
		std::cerr << "Given value should be >= 0! You provided: " << value << std::endl;
	}
}

void 
IgCircleSector::setOuterRadius (float value)
{
	if (value >= m_inner)
	{
		m_outer = value;
	}
	else
	{
		std::cerr << "Given value should be >= innerRadius! You provided: " << value << std::endl;
	}
}

void
IgCircleSector::setOpeningAngle (float value)
{
	if (value > 0. && value <= 360.)
	{
		m_opening = value;
	}
	else
	{
		std::cerr << "Given value should be > 0 and <= 360 degrees! You provided: " << value << std::endl;
	}
}

void 
IgCircleSector::setStartingAngle (float value)
{
	/* zero degree is at 3 o'clock */
	if (value >= 0. && value < 360.)
	{
		m_start = value;
	}
	else
	{
		std::cerr << "Given value should be >= 0 and < 360 degrees! You provided: " << value << std::endl;
	}
}

void
IgCircleSector::setCenter (const QPointF& value)
{
	m_center = value;
}

QRectF 
IgCircleSector::boundingRect (void) const
{
	return QRectF (m_center.x () - m_outer, m_center.y () - m_outer, 2. * m_outer, 2. * m_outer);
}

void 
IgCircleSector::drawOuter (QPainter* painter)
{
	QRectF rectangle (m_center.x () - m_outer, m_center.y () - m_outer, 2. * m_outer, 2. * m_outer);
	const int startAngle = (int) (m_start * 16.);
	const int spanAngle = (int) (m_opening * 16.);

	painter->drawArc (rectangle, startAngle, spanAngle);
}

void
IgCircleSector::drawInner (QPainter* painter)
{
	if (m_inner > 0.)
	{
		QRectF rectangle (m_center.x () - m_inner, m_center.y () - m_inner, 2. * m_inner, 2. * m_inner);
		// factor 16: the angle in drawArc can be defined as exactly as the 16th of a degree
		const int startAngle = (int) (m_start * 16.);
		const int spanAngle = (int) (m_opening * 16.);
	
		painter->drawArc (rectangle, startAngle, spanAngle);
	}
}

// returns x, y coordinates given polar coordinates
// @ param radialCoords = ( radius, angle, center)
// angle supposed to be in rad
QPointF
IgCircleSector::convertCoordinates (const float radius, const float phi, const QPointF& center /*= QPointF (0.f,0.f)*/) const
{
	QPointF cartesianCoords;
	cartesianCoords.setX (radius * cos (phi) + center.x ());
	cartesianCoords.setY (radius * sin (phi) + center.y ());
	return cartesianCoords;
}

void
IgCircleSector::drawSide (QPainter* painter)
{
// 	float start = static_cast<float>((int)(16. * m_start))/16.;
	float start = 2. * M_PI - (m_start * M_PI / 180.); // positive y axis is pointing downwards
	QLineF lineOne (convertCoordinates (m_inner, start, m_center), convertCoordinates (m_outer, start, m_center));
	
// 	float opening = static_cast<float>((int)(16. * m_opening))/16.;
	float opening = 2. * M_PI - (m_opening * M_PI / 180.);
	QLineF lineTwo (convertCoordinates (m_inner, start + opening, m_center), convertCoordinates (m_outer, start + opening, m_center));
	
	painter->drawLine (lineOne);
	painter->drawLine (lineTwo);
}

void
IgCircleSector::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	drawOuter (painter);
	drawInner (painter);
	drawSide (painter);
}
