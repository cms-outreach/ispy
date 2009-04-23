#ifndef IGGI_IG_TRACK_H
#define IGGI_IG_TRACK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/QtGUI/interface/ISpyItem.h"
# include <QPainter>
# include <QPainterPath>
# include <QPointF>
# include <QStyleOptionGraphicsItem>
# include <vector>
# include <QRectF>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

using namespace std;

class IgTrack : public ISpyItem
{

public:
    IgTrack (const vector<QPointF>& points, const vector<QPointF>& tangents /*= vector<QPointF>(0)*/,
	     /*const QPointF& position = QPointF (0., 0.),*/ 
	     const unsigned algorithm = AUTOMATIC, ISpyItem* parent  = 0);
    IgTrack (const vector<QPointF>& points, const unsigned algorithm = AUTOMATIC, ISpyItem* parent  = 0);
    ~IgTrack (void);
	
    virtual QRectF boundingRect (void) const;
    virtual void   paint (QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	
    void showPoints (bool value = true);
    void showInterpolation (bool value = true);
    void showTangents (bool value = false);
    void showBBox (bool value = false);
    
//     QPen pen (void) const;
//     void setPen (const QPen & pen);
	
    // can be used for bitwise OR-ing
    static unsigned const AUTOMATIC = 0;
    static unsigned const SPLINE    = 1;
    static unsigned const HERMITE   = 2;
    static unsigned const ALL       = 3;

private:
    QRectF calcBoundingRect (void) const;
    void   calcDistances (vector<double>& distances);
    void   calcParametricPoints (vector<double>& distances, vector<double>& x, vector<double>& y, vector<double>& paramSteps);	
    void   determineCoeffs (vector<double>& param, vector<double>& points, vector<vector<double> >& coefficients);
    double distance (const QPointF& a, const QPointF& b);
	
    void drawCurve (QPainter* painter);
    void drawHermiteCurve (QPainter* painter);
    void drawPoints (QPainter* painter);
    void drawSplineCurve (QPainter* painter);
    void drawTangents (QPainter* painter);

    void getHermitePath (QPainterPath& curvePath, unsigned index, double steps = 0.);	
    void getSplinesPath (QPainterPath& curvePath, vector<double>& coeffX,
                         vector<double>& coeffY, double intervalStart, 
                         double intervalEnd, double steps = 0.);

    void   tridiagonalSolve (const vector<double>& a, const vector<double>& b, vector<double>& c, vector<double>& d, vector<double>& x);
    double vectorLen (const QPointF& a);
	
    vector<QPointF> m_points;
    vector<QPointF> m_tangents;
    bool     m_showPoints;
    bool     m_showInterpolation;
    bool     m_showTangents;
    bool     m_showBBox;
    double   m_totalCurveLength;
    unsigned m_useAlgo;
    QRectF   m_boundingRect;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
#endif // IGGI_IG_TRACK_H
