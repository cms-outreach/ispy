#ifndef IGGI_IG_TRAPEZOID_H
#define IGGI_IG_TRAPEZOID_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IggiItem.h"
# include <QPainter>
# include <QStyleOptionGraphicsItem>
# include <QPointF>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgTrapezoid : public IggiItem
{

public:
	IgTrapezoid (float upperSide, float lowerSide, float height, const QPointF& center = QPointF (0., 0.), IggiItem* parent  = 0);
	~IgTrapezoid (void);
	
	virtual QRectF boundingRect (void) const;
	virtual void   paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	
	void setHeight    (float value);
	void setUpperSide (float value);
	void setLowerSide (float value);
	void setCenter    (const QPointF& value);

private:

	QPointF m_center;
	float   m_height;
	float   m_upper;
	float   m_lower;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IGGI_IG_TRAPEZOID_H
