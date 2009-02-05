#ifndef IGGI_IG_HITS_H
# define IGGI_IG_HITS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Iggi/interface/IggiItem.h"
# include <QPointF>
# include <QRectF>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgHits : public IggiItem
{
public:
    IgHits (const std::vector<QPointF>& points, IggiItem* parent  = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IgHits (void);
    
    virtual QRectF 	boundingRect (void) const;
    virtual void   	paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    void 		setColor (const QColor& value);

private:
    QRectF 		calcBoundingRect (void) const;
    void 		drawPoints (QPainter* painter,const QPen& pen);
    std::vector<QPointF> m_points;
    QRectF   		m_boundingRect;
    QColor   		m_color;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline void
IgHits::setColor (const QColor& value)
{
    m_color = value;
}

#endif // IGGI_IG_HITS_H
