#ifndef QT_GUI_IG_VISIBILITY_H
# define QT_GUI_IG_VISIBILITY_H

//<<<<<< INCLUDES                                                       >>>>>>

#include <QMetaType>
#include <QPointF>
#include <QPolygonF>
#include <QVector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QPainter;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgVisibility
{
public:
    enum EditMode { Editable, ReadOnly };

    IgVisibility(int starCount = 1, int maxStarCount = 5);

    void paint(QPainter *painter, const QRect &rect,
	       const QPalette &palette, EditMode mode) const;
    QSize sizeHint() const;
    int starCount() const { return myStarCount; }
    int maxStarCount() const { return myMaxStarCount; }
    void setStarCount(int starCount) { myStarCount = starCount; }
    void setMaxStarCount(int maxStarCount) { myMaxStarCount = maxStarCount; }

private:
    QPolygonF starPolygon;
    QPolygonF diamondPolygon;
    int myStarCount;
    int myMaxStarCount;
};

Q_DECLARE_METATYPE(IgVisibility)


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_IG_VISIBILITY_H
