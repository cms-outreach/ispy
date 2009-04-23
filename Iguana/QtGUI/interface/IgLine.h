#ifndef IGGI_IG_LINE_H
# define IGGI_IG_LINE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/QtGUI/interface/ISpyItem.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgLine : public ISpyItem
{
public:
    IgLine (const QPointF & p1, const QPointF & p2, ISpyItem* parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IgLine (void);
    
    virtual QRectF 	boundingRect (void) const;
    virtual void   	paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    void 		setColor (const QColor& value);

private:
    QRectF 		calcBoundingRect (void) const;
    void 		drawLine (QPainter* painter,const QPen& pen);
    std::vector<QPointF> m_points;
    QRectF   		m_boundingRect;
    QColor   		m_color;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline void
IgLine::setColor (const QColor& value)
{
    m_color = value;
}

#endif // IGGI_IG_LINE_H
