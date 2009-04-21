//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgVisibility.h"
#include <QtGui>
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

const int PaintingScaleFactor = 20;

IgVisibility::IgVisibility (int starCount, int maxStarCount)
{
    myStarCount = starCount;
    myMaxStarCount = maxStarCount;

    starPolygon << QPointF(1.0, 0.5);
    for (int i = 1; i < 5; ++i)
	starPolygon << QPointF(0.5 + 0.5 * cos(0.8 * i * 3.14),
			       0.5 + 0.5 * sin(0.8 * i * 3.14));

    diamondPolygon << QPointF(0.4, 0.5) << QPointF(0.5, 0.4)
		   << QPointF(0.6, 0.5) << QPointF(0.5, 0.6)
		   << QPointF(0.4, 0.5);
}

QSize 
IgVisibility::sizeHint() const
{
    return PaintingScaleFactor * QSize(myMaxStarCount, 1);
}

void 
IgVisibility::paint(QPainter *painter, const QRect &rect,
		    const QPalette &palette, EditMode mode) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    if (mode == Editable) {
	painter->setBrush(palette.highlight());
    } else {
	painter->setBrush(palette.foreground());
    }

    int yOffset = (rect.height() - PaintingScaleFactor) / 2;
    painter->translate(rect.x(), rect.y() + yOffset);
    painter->scale(PaintingScaleFactor, PaintingScaleFactor);

    for (int i = 0; i < myMaxStarCount; ++i) {
	if (i < myStarCount) {
	    painter->drawPolygon(starPolygon, Qt::WindingFill);
	} else if (mode == Editable) {
	    painter->drawPolygon(diamondPolygon, Qt::WindingFill);
	}
	painter->translate(1.0, 0.0);
    }

    painter->restore();
}
