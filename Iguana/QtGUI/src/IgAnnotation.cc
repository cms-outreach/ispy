//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgAnnotation.h"
#include <QPainter>
#include <QFont>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgAnnotation::IgAnnotation (const QString& text, const QPoint& pos, const QFont& font, ISpyItem* parent)
    : ISpyItem (parent),
      m_text (text),
      m_pos (pos),
      m_font (font)
{
    m_boundingRect = calcBoundingRect ();
}

IgAnnotation::~IgAnnotation (void)
{}

QRectF 
IgAnnotation::boundingRect (void) const
{ 
    return m_boundingRect;
}

QRectF
IgAnnotation::calcBoundingRect (void) const
{
    QFontMetrics fontm (m_font);
    return fontm.boundingRect (m_text);
}

void 
IgAnnotation::drawAnnotation (QPainter* painter)
{
    painter->drawText(m_pos, m_text);
}

void
IgAnnotation::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    bool showBoundingBox = false;
    painter->setFont (m_font);
    painter->setPen (pen ());
    
    drawAnnotation (painter);
    if (showBoundingBox)
	painter->drawRect (calcBoundingRect ());
}
