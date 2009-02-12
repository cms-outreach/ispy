#ifndef IGGI_IG_ANNOTATION_H
# define IGGI_IG_ANNOTATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Iggi/interface/IggiItem.h"
# include <QFont>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgAnnotation : public IggiItem
{
public:
    IgAnnotation (const QString& text, const QPoint& pos,
		  const QFont& font, IggiItem* parent  = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IgAnnotation (void);
    
    virtual QRectF 	boundingRect (void) const;
    virtual void   	paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

private:
    QRectF 		calcBoundingRect (void) const;
    void 		drawAnnotation (QPainter* painter);
    QString 		m_text;
    QPoint		m_pos;
    QFont		m_font;
    QRectF   		m_boundingRect;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_ANNOTATION_H
