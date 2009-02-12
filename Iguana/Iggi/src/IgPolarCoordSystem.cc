//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/Iggi/interface/IgPolarCoordSystem.h"
#include <QPen>

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

IgPolarCoordSystem::IgPolarCoordSystem (IggiItem * parent/* = 0*/) :
	IggiItem          (parent),
	m_startRadius     (0.),
	m_endRadius       (170.),
	m_lineAngle       (22.5f * M_PI/180.f),
	m_center          (QPointF (0., 0.)),
	m_color           (Qt::lightGray),
	m_unitLabel       (QString ("GeV")),
	m_ratioE2U        (10.),   // how much energy is seen as a "unit"
	m_ratioP2E        (1.),    // how many pixel per energy (for zooming)
	m_showUnitLabel   (true),
	m_showAngleLabels (true),
	m_showMinorLines  (true)
{}

IgPolarCoordSystem::~IgPolarCoordSystem (void)
{}

void
IgPolarCoordSystem::setLineAngle (float value)
{
	// don't do anything for ridiculous values
	if ((value < (5. * M_PI/180. - 0.001)) || (value > (360. * M_PI/180. + 0.001)))
	{
		m_lineAngle = 0.;
		std::cerr << "The angle between the lines should be between 5 and 360 degrees! (in rad)" << std::endl;
	}
	else
	{
		m_lineAngle = value;
	}
	update ();
}

void
IgPolarCoordSystem::zoomIn ()
{
	setP2Eratio (m_ratioP2E * 2.);
}

void
IgPolarCoordSystem::zoomOut ()
{
	setP2Eratio (m_ratioP2E * 0.5);
}

QRectF 
IgPolarCoordSystem::boundingRect (void) const
{
	const float labelAndRadius = m_endRadius + 30.;
	return QRectF (m_center.x () - labelAndRadius, m_center.y () - labelAndRadius, 2. * labelAndRadius, 2. * labelAndRadius);
}

// returns x, y coordinates given polar coordinates
// @ param radialCoords = ( radius, angle)
// angle supposed to be in rad
QPointF
IgPolarCoordSystem::convertCoordinates (const float radius, const float phi, const QPointF& center /* = QPointF (0.f,0.f)*/) const
{
	QPointF cartesianCoords;
	cartesianCoords.setX (radius * cos (phi) + center.x ());
	cartesianCoords.setY (radius * sin (phi) + center.y ());
	
	// in order to avoid drawing artifacts, like tilted lines although they should be straight
	const float x = fabs (cartesianCoords.x ());
	const float y = fabs (cartesianCoords.y ());
	if (x < 0.0001 && x > 0.) cartesianCoords.setX (0.);
	if (y < 0.0001 && y > 0.) cartesianCoords.setY (0.);
	return cartesianCoords;
}

void
IgPolarCoordSystem::drawCircles (QPainter* painter, const QPen& pen)
{
	painter->setPen (pen);
	float distance = m_ratioE2U * m_ratioP2E;
	
	const unsigned lineCount = (unsigned)((m_endRadius - m_startRadius) / distance);
	
	for (unsigned i = 0; i <= lineCount; i++)
	{
		painter->drawEllipse (m_center, m_startRadius + i * distance, m_startRadius + i * distance);
	}
}

void
IgPolarCoordSystem::drawMajorLines (QPainter* painter, const QPen& pen)
{
	painter->setPen (pen);
	// draw the principal lines first
	for (float i = 0.; i < 2. * M_PI; i += M_PI/4.)
	{
		painter->drawLine (m_center, convertCoordinates (m_endRadius, i, m_center));
	}
}

void
IgPolarCoordSystem::drawMinorLines (QPainter* painter, const QPen& pen)
{
	painter->setPen (pen);
	QPointF temp;
	// draw dotted lines for the rest of the angles
	if (m_lineAngle != 0.)
	{
		for (float angle = m_lineAngle; angle < (2. * M_PI); angle += m_lineAngle)
		{
			temp = convertCoordinates (m_endRadius, angle, m_center);
			temp.setY (-temp.y()); // since it's left handed coordinate system
			painter->drawLine (m_center, temp);
		}
	}
}

void
IgPolarCoordSystem::drawUnitLabel (QPainter* painter, const QPen& pen)
{
	painter->setPen (pen);
	const float scaleLen = m_ratioE2U * m_ratioP2E;
	// used for calculating the box's size in which the label is positioned
	// width: digits: 7 pixel, points: 5 pixel, gaps: 5 pixel, chars: 8 pixel
	// hight of all: 10 pixel
	const int numberSize = ((int) (((log (m_ratioE2U)/log (10.)) < 0.) ? 0. : (log (m_ratioE2U)/log (10.))) + 1)*7 + 5 + 2*7 + 5;
	QRectF labelBox;
	QSizeF labelBoxSize (8. * m_unitLabel.size () + numberSize, 10.);
	// draw the label relative to the local center
	QPointF labelPoint (m_center.x () + m_endRadius - scaleLen - 5., m_center.y () + m_endRadius - 5.);
		
	const float lineOffset = 3.;
	const float labX = labelPoint.x ();
	const float labY = labelPoint.y ();
	// little piece at the beginning of the scale
	painter->drawLine (QPointF (labX, labY + lineOffset), QPointF (labX, labY - lineOffset));
	// middle line
	painter->drawLine (QPointF (labX, labY), QPointF (labX + scaleLen, labY));	
	// little piece at the end of the scale
	painter->drawLine (QPointF (labX + scaleLen, labY + lineOffset), QPointF (labX + scaleLen, labY - lineOffset));	
	QString finalLabel = QString (floatToString (m_ratioE2U,2).c_str ()) + QString (" ") + m_unitLabel;
	
	labelBox.setTopLeft (QPointF (labX + scaleLen - labelBoxSize.width (), labY - (lineOffset + 2.f) - labelBoxSize.height ()));
	labelBox.setSize (labelBoxSize);
	painter->drawText (labelBox, Qt::AlignCenter, finalLabel);
}

void
IgPolarCoordSystem::drawAngleLabels (QPainter* painter, const QPen& pen)
{
	painter->setPen (pen);
	const float textGap = 5.;
	const float dist = m_endRadius + textGap;
	
	// we use rotate, because we also want to turn the labels, therefore we
	// need to save the painter's state and restore it later on
	// remember: left-handed coordinate system
	QPointF temp = convertCoordinates (dist, 0.);
	
	painter->save ();
	painter->translate (m_center);
	painter->drawText (temp, QString ("0.0"));
	painter->rotate (-45);
	painter->drawText (temp, QString ("45.0"));
	painter->rotate (-45);
	painter->drawText (temp, QString ("90.0"));
	painter->restore ();
		
	// also place the dotted lines' labels
	if (m_lineAngle != 0.f && m_showMinorLines)
	{
		painter->save ();
		painter->translate (m_center);
		
		float text;
		const float end = -M_PI/2. + 0.001;
		const float deg45 = -M_PI/4.;
		
		for (float i = -m_lineAngle; i > end ; i -= m_lineAngle)
		{
			// skip the one at 45 degrees
			if (!(fabs (i - deg45) < 0.001))
			{
				text = fabs (i) * 180. / M_PI;
				painter->rotate (-m_lineAngle * 180. / M_PI);
				painter->drawText (convertCoordinates (dist, 0.), QString (floatToString (text).c_str ()));
			}
			else
			{
				painter->rotate (-m_lineAngle * 180. / M_PI);
			}
		}
		painter->restore ();
	}
}

std::string
IgPolarCoordSystem::floatToString (float a, unsigned precision /* = 1 */) const
{
	std::ostringstream s;
	// one digit after decimal point
	s.setf (std::ios::fixed, std::ios::floatfield);
	s.precision (precision);
	s << a;
	return s.str();
}

void
IgPolarCoordSystem::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	drawCircles (painter, QPen (m_color));
	drawMajorLines (painter, QPen (m_color));
	if (m_showMinorLines)  drawMinorLines (painter, QPen (m_color, 0, Qt::DotLine));
	if (m_showUnitLabel)   drawUnitLabel (painter, QPen ());
	if (m_showAngleLabels) drawAngleLabels (painter, QPen (m_color));
}
