#ifndef IGGI_IG_CIRCLESECTOR_H
#define IGGI_IG_CIRCLESECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IggiItem.h"
# include <QPainter>
# include <QStyleOptionGraphicsItem>
# include <QPointF>
# include <QLineF>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgCircleSector : public IggiItem
{

public:
	IgCircleSector (float innerRadius, float outerRadius, float openingAngle, float startAngle = 0, 
	               const QPointF& center = QPointF (0., 0.), IggiItem* parent  = 0);
	~IgCircleSector (void);
	
	virtual QRectF boundingRect (void) const;
	virtual void   paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	
	void setInnerRadius   (float value);
	void setOuterRadius   (float value);
	void setOpeningAngle  (float value);
	void setStartingAngle (float value);
	void setCenter        (const QPointF& value);

private:
	void drawOuter (QPainter* painter);
	void drawInner (QPainter* painter);
	void drawSide  (QPainter* painter);
	
	QPointF convertCoordinates (const float radius, const float phi, const QPointF& center = QPointF (0.f,0.f)) const;

	QPointF m_center;
	float   m_inner;
	float   m_outer;
	float   m_opening;
	float   m_start;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IGGI_IG_CIRCLESECTOR_H
