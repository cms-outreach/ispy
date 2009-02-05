//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/Iggi/interface/IgRZCoordSystem.h"
#include <QSizeF>
#include <math.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

const QPointF IgRZCoordSystem::constCorrection = QPointF (10.f, 5.f);
const QSizeF  IgRZCoordSystem::labelBoxSize = QSizeF (20.f, 10.f);

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgRZCoordSystem::IgRZCoordSystem (IggiItem * parent /* = 0*/) :
	IggiItem              (parent),
	m_maxEta              (5.f),
	m_maxWidth            (12.f),
	m_maxHeight           (6.f),
	m_deltaEta            (0.3f),
	m_showMeterLines      (true),
	m_showCentimeterLines (false),
	m_showMillimeterLines (false),
	m_center              (QPointF (0.f, 0.f)),
	m_color               (Qt::lightGray),
	m_needVariablesUpdate (true),
	m_marginEta           (0.f),
	m_scaleFactor         (20.f),
	m_showEtaLabels       (true),
	m_showEtaLines        (true),
	m_viewportHeight      (400.f),
	m_viewportWidth       (400.f)
{
	horizontal.setP1 (QPointF (0.f, (m_viewportHeight/2.f) - 1.6f * labelBoxSize.height ()));
	horizontal.setP2 (QPointF (1.f, (m_viewportHeight/2.f) - 1.6f * labelBoxSize.height ()));
	vertical.setP1   (QPointF ((m_viewportWidth/2.f) - 1.6f * labelBoxSize.width (), 0.f));
	vertical.setP2   (QPointF ((m_viewportWidth/2.f) - 1.6f * labelBoxSize.width (), 1.f));
}

IgRZCoordSystem::~IgRZCoordSystem (void)
{}

void
IgRZCoordSystem::zoomIn ()
{
	setScaleFactor (m_scaleFactor * 2.);
}

void
IgRZCoordSystem::zoomOut ()
{
	setScaleFactor (m_scaleFactor * 0.5);
}

QRectF 
IgRZCoordSystem::boundingRect (void) const
{
// 	const float cornerX = m_center.x () - m_maxWidth * m_scaleFactor - 2.f * labelBoxSize.width ();
// 	const float cornerY = m_center.y () - m_maxHeight * m_scaleFactor - 2.f * labelBoxSize.height ();
// 	const float widthX = 2.f * m_maxWidth * m_scaleFactor + 4.f * labelBoxSize.width ();
// 	const float widthY = 2.f * m_maxHeight * m_scaleFactor + 4.f * labelBoxSize.height ();
// 	return QRectF (cornerX, cornerY, widthX, widthY);
	return QRectF (-(m_viewportWidth/2.f), -(m_viewportHeight/2.f), m_viewportWidth, m_viewportHeight);
}

// returns x, y coordinates given polar coordinates
// @ param radialCoords = ( radius, angle, center)
// angle supposed to be in rad
QPointF
IgRZCoordSystem::convertCoordinates (const float radius, const float phi, const QPointF& center) const
{
	QPointF cartesianCoords;
	cartesianCoords.setX (radius * cos (phi) + center.x ());
	cartesianCoords.setY (radius * sin (phi) + center.y ());
	return cartesianCoords;
}

std::string
IgRZCoordSystem::floatToString (float a) const
{
	std::ostringstream s;
	// one digit after decimal point
	s.setf (std::ios::fixed, std::ios::floatfield);
	s.precision (1);
	s << a;
	return s.str();
}

void
IgRZCoordSystem::drawAxes (QPainter *painter, const QPen& pen)
{
	painter->setPen (pen);
	
	// draw X-Axis
	QPointF firstPoint (-vertical.x1 (), 0.f);
	QPointF secondPoint (vertical.x1 (), 0.f);
	painter->drawLine (firstPoint + m_center, secondPoint + m_center);
	
	// draw Y-Axis
	firstPoint.setX (0.f);
	firstPoint.setY (-horizontal.y1 ());
	secondPoint.setX (0.f);
	secondPoint.setY (horizontal.y1 ());
	painter->drawLine (firstPoint + m_center, secondPoint + m_center);
}

void 
IgRZCoordSystem::drawEtaLines (QPainter* painter, const QPen& pen)
{
	painter->setPen (pen);
	
	QPointF secondPoint;
	
	// the coordinate systems's quarters are symmetrical, only differing in signs
	for (int signY = -1; signY <= 1; signY += 2)
	{
		for (int signX = -1; signX <= 1; signX += 2)
		{
			// draw eta lines in a quarter
			for (float eta = 0.f; eta <= m_maxEta; eta += m_deltaEta)
			{
				secondPoint = getPointOnRectangle (eta, signX, signY, (isUpperEdgeEta (eta)) ? horizontal : vertical);
				painter->drawLine (m_center, secondPoint + m_center);
			}
		}
	}
}

QPointF
IgRZCoordSystem::getPointOnRectangle (const float& eta, const int& signX, const int& signY, const QLineF& orientedLine) const
{
	// this method finds the end point of the eta line on the rectangle defined
	// by m_maxHeight and m_maxWidth (which is the border of the whole
	// structure. For that two static lines (vertical and horizontal) are
	// compared, and the more convenient one is passed to this method. Then the
	// method uses this 'orientedLine' to determine the point on the rectangle
	// and returns it as result.
	
	const QPointF temp = convertCoordinates (1.f, etaToTheta (eta));
	const QLineF movingLine (QPointF (0.f, 0.f), temp);
	QPointF result (0.f, 0.f);
	
	movingLine.intersect (orientedLine, &result);
	result.setX (result.x () * signX);
	result.setY (result.y () * signY);
	
	return result;
}

void
IgRZCoordSystem::drawHeightLines (QPainter* painter, const QPen& pen, float delta)
{
	painter->setPen (pen);
	
	QPointF firstPoint;
	QPointF secondPoint;

	const float halfHeight = horizontal.y1 ();
	const float halfWidth = vertical.x1 ();
	
	firstPoint.setX (-halfWidth);
	secondPoint.setX (halfWidth);
	
	// draw horizontal lines, first the ones on the positive side, then the
	// ones on the negative side.
	for (int sign = 1 ; sign >= -1; sign -= 2)
	{
		for (float height = 0.f; height < halfHeight; height += (delta * m_scaleFactor))
		{
			firstPoint.setY (sign * height);
			secondPoint.setY (sign * height);
			painter->drawLine (firstPoint + m_center, secondPoint + m_center);
		}
	}
}

void 
IgRZCoordSystem::drawWidthLines (QPainter* painter, const QPen& pen, float delta)
{
	painter->setPen (pen);
	
	QPointF firstPoint;
	QPointF secondPoint;
	
	const float halfHeight = horizontal.y1 ();
	const float halfWidth = vertical.x1 ();
	
	firstPoint.setY (-halfHeight);
	secondPoint.setY (halfHeight);
	
	// draw vertical lines, first the ones on the positive side, then the
	// ones on the negative side.
	for (int sign = 1 ; sign >= -1; sign -= 2)
	{
		for (float width = 0.f; width < halfWidth; width += (delta * m_scaleFactor))
		{
			firstPoint.setX (sign * width);
			secondPoint.setX (sign * width);
			painter->drawLine (firstPoint + m_center, secondPoint + m_center);
		}
	}
}

void 
IgRZCoordSystem::drawEtaLabels (QPainter* painter, const QPen& pen)
{
	// Draw labels for the eta lines. Text fields can be specified by the top
	// left corner and the rectangle size, therefore we need some sort of
	// correction to center the text. Further more we need an offset so that
	// the lines and texts dont coincide.
	painter->setPen (pen);
	
	// the label's top left point
	QPointF topLeftPoint;
	QPointF correctingOffset;

	QRectF labelBox;
	QRectF prevLabelBox;
	
	float distFactor = (m_deltaEta < 0.2f) ? 2.f : 1.f; // determines the distance between the labels, if too close, space it up
	float etaStart = m_deltaEta * distFactor;           // used in order not to draw "0.0" 4-times
	
	// iterate over the signs to calculate all 4 coordinate system regions
	for (int signY = -1; signY <= 1; signY += 2)
	{
		for (int signX = -1; signX <= 1; signX += 2)
		{
			for (float eta = etaStart; eta <= m_maxEta; eta = eta + m_deltaEta * distFactor)
			{
				// set the correcting offset depending on which edge the eta
				// line is pointing to
				const bool upperEdge = isUpperEdgeEta (eta);
				const float correctionInX = ((upperEdge) ? 0.f : labelBoxSize.width ()) * signX;
				const float correctionInY = ((upperEdge) ? labelBoxSize.height () : 0.f) * signY;
				correctingOffset.setX (correctionInX);
				correctingOffset.setY (correctionInY);
				
				topLeftPoint = getPointOnRectangle (eta, signX, signY, (isUpperEdgeEta (eta)) ? horizontal : vertical);
				topLeftPoint = topLeftPoint - constCorrection + correctingOffset;
				
				labelBox.setTopLeft (topLeftPoint + m_center);
				labelBox.setSize (labelBoxSize);
				// if the current label box is intersecting the previous one,
				// dont draw it
				if (labelBox.intersects (prevLabelBox)) continue;
				prevLabelBox = labelBox;
				
				painter->drawText (labelBox, Qt::AlignCenter, QString (floatToString (signX * eta).c_str ()));
			}
			etaStart = 0.f;
		}
		etaStart = m_deltaEta * distFactor;
	}
}

void
IgRZCoordSystem::drawMeterLines (QPainter* painter, const QPen& pen)
{
	
	drawHeightLines (painter, pen, 1.f);
	drawWidthLines (painter, pen, 1.f);
}

void
IgRZCoordSystem::drawCentimeterLines (QPainter* painter, const QPen& pen)
{
	
	drawHeightLines (painter, pen, 0.01f);
	drawWidthLines (painter, pen, 0.01f);
}

void
IgRZCoordSystem::drawMillimeterLines (QPainter* painter, const QPen& pen)
{
	
	drawHeightLines (painter, pen, 0.001f);
	drawWidthLines (painter, pen, 0.001f);
}

void
IgRZCoordSystem::updateVariables ()
{
	// update marginEta, i.e. determine the eta of the angle of the corner of
	// the rectangle which is defined by maxWidth and maxHeight :)
	const float theta = atan (m_viewportHeight/m_viewportWidth);
	m_marginEta = thetaToEta (theta);
	
	// update the size of the confining rectangle
	const float factor = 1.6f;
	horizontal.setP1 (QPointF (0.f, (m_viewportHeight/2.f) - factor * labelBoxSize.height ()));
	horizontal.setP2 (QPointF (1.f, (m_viewportHeight/2.f) - factor * labelBoxSize.height ()));
	vertical.setP1   (QPointF ((m_viewportWidth/2.f) - factor * labelBoxSize.width (), 0.f));
	vertical.setP2   (QPointF ((m_viewportWidth/2.f) - factor * labelBoxSize.width (), 1.f));
	
	// possible other updates here...
	
	// variables are updated
	m_needVariablesUpdate = false;
}

void
IgRZCoordSystem::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (m_needVariablesUpdate) updateVariables ();
	
	QPen solidPen (m_color);
	QPen dashedPen (m_color, 0, Qt::DashLine);
	QPen dottedPen (m_color, 0, Qt::DotLine);
		
	drawAxes (painter, solidPen);
	if (m_showMeterLines) drawMeterLines (painter, solidPen);
	if (m_showCentimeterLines) drawCentimeterLines (painter, dashedPen);
	if (m_showMillimeterLines) drawMillimeterLines (painter, dottedPen);
	if (m_showEtaLines) drawEtaLines (painter, dottedPen);
	if (m_showEtaLabels) drawEtaLabels (painter, solidPen);
}
