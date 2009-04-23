//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/QtGUI/interface/IgRectangle.h"
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

IgRectangle::IgRectangle (float width, float height, const QPointF& center /* = QPointF (0., 0.)*/, ISpyItem* parent /* = 0 */) :
	ISpyItem (parent),
	m_center (center),
	m_height (height),
	m_width  (width)
{}

IgRectangle::~IgRectangle (void)
{}

void 
IgRectangle::setHeight (float value)
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
IgRectangle::setWidth (float value)
{
	if (value > 0.)
	{
		m_width = value;
	}
	else
	{
		std::cerr << "Given value should be > 0! You provided: " << value << std::endl;
	}
}

void 
IgRectangle::setCenter (const QPointF& value)
{
	m_center = value;
}

QRectF 
IgRectangle::boundingRect (void) const
{
	return QRectF (m_center.x () - m_width/2., m_center.y () - m_height/2., m_width, m_height);
}

void
IgRectangle::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPointF topLeft (m_center.x () - m_width/2., m_center.y () - m_height/2.);
	QRectF  rect (topLeft, QSizeF (m_width, m_height));
	painter->drawRect (rect);
}
