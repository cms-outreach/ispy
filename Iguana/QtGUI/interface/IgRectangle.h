#ifndef IGGI_IG_RECTANGLE_H
#define IGGI_IG_RECTANGLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ISpyItem.h"
# include <QPainter>
# include <QStyleOptionGraphicsItem>
# include <QPointF>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgRectangle : public ISpyItem
{

public:
	IgRectangle (float width, float height, const QPointF& center = QPointF (0., 0.), ISpyItem* parent  = 0);
	~IgRectangle (void);
	
	virtual QRectF boundingRect (void) const;
	virtual void   paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	
	void setHeight (float value);
	void setWidth  (float value);
	void setCenter (const QPointF& value);

private:

	QPointF m_center;
	float   m_height;
	float   m_width;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IGGI_IG_RECTANGLE_H
