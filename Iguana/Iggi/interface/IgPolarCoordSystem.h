#ifndef IG_TEST_IG_POLARCOORDSYSTEM_H
# define IG_TEST_IG_POLARCOORDSYSTEM_H

//<<<<<< INCLUDES                                                       >>>>>>

// # include <QAbstractGraphicsShapeItem>
# include "IggiItem.h"
# include <QPainter>
# include <QStyleOptionGraphicsItem>
# include <QPointF>
# include <iostream>
# include <sstream>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// class IgPolarCoordSystem : public QAbstractGraphicsShapeItem
class IgPolarCoordSystem : public IggiItem
{
  
public:
	IgPolarCoordSystem (IggiItem * parent = 0);
	~IgPolarCoordSystem (void);
	
	virtual QRectF boundingRect (void) const;
	virtual void   paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	virtual void   zoomIn (void);
	virtual void   zoomOut (void);
	
	void setCenter (const QPointF& value);
	void setColor (const QColor& value);
	void setEndRadius (float value);
	void setP2Eratio (float value);
	void setE2Uratio (float value);
	void setLineAngle (float value);
	void setStartRadius (float value);
	void setUnitLabel (const QString& value);
	
	void showUnitLabel (bool value = true);
	void showAngleLabels (bool value = true);
	void showMinorAngles (bool value = true);

private:

	void        drawCircles (QPainter* painter, const QPen& pen);
	void        drawMajorLines (QPainter* painter, const QPen& pen);
	void        drawMinorLines (QPainter* painter, const QPen& pen);
	void        drawUnitLabel (QPainter* painter, const QPen& pen);
	void        drawAngleLabels (QPainter* painter, const QPen& pen);
	
	QPointF     convertCoordinates (const float radius, const float phi, const QPointF& center = QPointF (0., 0.)) const;
	std::string floatToString (float a, unsigned precision = 1) const;
	
	float    m_startRadius;
	float    m_endRadius;
	float    m_lineAngle;
	QPointF  m_center;
	QColor   m_color;
	QString  m_unitLabel;// shown in the corner
	float    m_ratioE2U; // ratio of Energy to Unit 
	float    m_ratioP2E; // ratio of Energy to Pixel
	bool     m_showUnitLabel;
	bool     m_showAngleLabels;
	bool     m_showMinorLines;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

inline void 
IgPolarCoordSystem::showUnitLabel (bool value /* = true*/)
{
	m_showUnitLabel = value;
	update ();
}

inline void 
IgPolarCoordSystem::showAngleLabels (bool value /*= true*/)
{
	m_showAngleLabels = value;
	update ();
}

inline void 
IgPolarCoordSystem::showMinorAngles (bool value /*= true*/)
{
	m_showMinorLines = value;
	update ();
}

inline void
IgPolarCoordSystem::setStartRadius (float value)
{
	if (value < m_endRadius && value >= 0.)
	{
		m_startRadius = value;
		update ();
	}
}

inline void
IgPolarCoordSystem::setEndRadius (float value)
{
	if (value >= m_startRadius)
	{
		m_endRadius = value;
		update ();
	}
}

inline void
IgPolarCoordSystem::setColor (const QColor& value)
{
	m_color = value;
}

inline void
IgPolarCoordSystem::setCenter (const QPointF& value)
{
	m_center = value;
}

inline void
IgPolarCoordSystem::setUnitLabel (const QString& value)
{
	m_unitLabel = value;
	update ();
}

inline void
IgPolarCoordSystem::setE2Uratio (float value)
{
	if (value > 0.)
	{
		m_ratioE2U = value;
		update ();
	}
}

inline void
IgPolarCoordSystem::setP2Eratio (float value)
{
	if (value > 0.)
	{
		m_ratioP2E = value;
		update ();
	}
}

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEST_IG_POLARCOORDSYSTEM_H
