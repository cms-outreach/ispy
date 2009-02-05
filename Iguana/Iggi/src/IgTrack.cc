//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/Iggi/interface/IgTrack.h"
#include <iostream>
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

// using namespace std;

IgTrack::IgTrack (const vector<QPointF>& points, const vector<QPointF>& tangents /*= vector<QPointF>(0)*/,
		  /*const QPointF& position = QPointF (0., 0.),*/ const unsigned algo /*= AUTOMATIC*/, IggiItem* parent /*= 0*/) 
    : IggiItem            (parent),
      m_points            (points),
      m_tangents          (tangents),
      m_showPoints        (true),
      m_showInterpolation (true),
      m_showTangents      (false),
      m_showBBox 	  (false),
      m_totalCurveLength  (0.),
      m_useAlgo           (algo)
{
    // NOTE if number of points are changed, calcBoundingRect method should
    // called again
    m_boundingRect = calcBoundingRect ();
}

IgTrack::IgTrack (const vector<QPointF>& points, const unsigned algo /*= AUTOMATIC*/, IggiItem* parent /*= 0*/) :
    IggiItem            (parent),
    m_points            (points),
    m_showPoints        (true),
    m_showInterpolation (true),
    m_showTangents      (false),
    m_totalCurveLength  (0.),
    m_useAlgo           (algo)
{
    // NOTE if number of points are changed, calcBoundingRect method should
    // called again
    m_boundingRect = calcBoundingRect ();
}

IgTrack::~IgTrack (void)
{}

QRectF 
IgTrack::boundingRect (void) const
{ 
    return m_boundingRect;
}

QRectF
IgTrack::calcBoundingRect (void) const
{
    // this method builds rectangles with the given points and unites them to
    // one big rectangle. This might exclude sometimes some small parts of the
    // curve, since the curves are not limited to the convex hull.
    QRectF rec;
    unsigned n = m_points.size ();
	
    if (n > 1)
    {
	rec.setTopLeft (m_points[0]);
	rec.setBottomRight (m_points[1]);
	QPointF lastUsed = m_points[1];
		
	if (n > 2)
	{
	    for (unsigned i = 2; i < m_points.size (); i++)
	    {
		if (!rec.contains (m_points[i]))
		{
		    QRectF temp (m_points[i], lastUsed);
		    rec = rec.united (temp);
		    lastUsed = m_points[i];
		}
	    }
	}
    }
    rec = rec.normalized ();
	
    // give the rectange a bit of a brim
    float brim = 1.0;
    rec.setTop ((rec.top () > 0.) ? (rec.top () + brim) : (rec.top () - brim));
    rec.setBottom ((rec.bottom () > 0.) ? (rec.bottom () + brim) : (rec.bottom () - brim));
    rec.setLeft ((rec.left () > 0.) ? (rec.left () + brim) : (rec.left () - brim));
    rec.setRight ((rec.right () > 0.) ? (rec.right () + brim) : (rec.right () - brim));

    return rec;
}

void 
IgTrack::showPoints (bool value /*= true*/)
{
    m_showPoints = value;
    update ();
}

void 
IgTrack::showInterpolation (bool value /*= true*/)
{
    m_showInterpolation = value;
    update ();
}

void 
IgTrack::showTangents (bool value /*= false*/)
{
    m_showTangents = value;
    update ();
}

void 
IgTrack::showBBox (bool value /*= false*/)
{
    m_showBBox = value;
    update ();
}

void 
IgTrack::drawPoints (QPainter* painter)
{
    const float circle = 4.;
    for (unsigned i = 0; i < m_points.size (); i++)
    {
	const float x_corner = m_points[i].x () - circle/2.;
	const float y_corner = m_points[i].y () - circle/2.;
	painter->drawEllipse (QRectF (x_corner, y_corner, circle, circle));
    }
}

void 
IgTrack::drawCurve (QPainter* painter)
{
    // if more algorithms are added, this part could be changed to "bitfield
    // checking". Example: 0...000101 -> use 1st and 3rd algorithm
    switch (m_useAlgo)
    {
    case SPLINE:
	drawSplineCurve (painter);
	break;
			
    case HERMITE:
	if (!m_tangents.empty ()) drawHermiteCurve (painter);
	break;
			
    case ALL:
	drawSplineCurve (painter);
	if (!m_tangents.empty ()) drawHermiteCurve (painter);
	break;
			
    default: //AUTOMATIC
	if (!m_tangents.empty ()) drawHermiteCurve (painter);
	else drawSplineCurve (painter);
	break;
    }
}

void 
IgTrack::drawSplineCurve (QPainter* painter)
{
    QPainterPath curvePath;
    vector<double> xPoints;
    vector<double> yPoints;
    vector<double> paramSteps;
    vector<double> distances (m_points.size (), 0.);
	
    calcDistances (distances);
    calcParametricPoints (distances, xPoints, yPoints, paramSteps);

    vector<vector<double> > coeffX;
    vector<vector<double> > coeffY;
	
    determineCoeffs (paramSteps, xPoints, coeffX);
    determineCoeffs (paramSteps, yPoints, coeffY);
	
    vector<double> tempCoeffX (4, 0.);
    vector<double> tempCoeffY (4, 0.);
	
    // there are n points and n-1 intervals
    for (unsigned j = 0; j < m_points.size () - 1; j++)
    {
	double s_j     = paramSteps[j];
	double s_jplus = paramSteps[j+1];
		
	//since coeffX and coeffY are column major we need two new vectors containing the values of the ith column
	for (unsigned i = 0; i < 4; i++)
	{
	    tempCoeffX[i] = coeffX[i][j];
	    tempCoeffY[i] = coeffY[i][j];
	}
		
	getSplinesPath (curvePath, tempCoeffX, tempCoeffY, s_j, s_jplus);
    }
    painter->drawPath (curvePath);
}

void
IgTrack::drawHermiteCurve (QPainter* painter)
{
    QPainterPath curvePath;
    // move to the first position
    curvePath.moveTo (m_points[0].x (), m_points[0].y ());

    // there are n points and n-1 intervals
    for (unsigned j = 0; j < m_points.size () - 1; j++)
    {
	getHermitePath (curvePath, j/*, 50*/);
    }
    painter->drawPath (curvePath);
}

void
IgTrack::drawTangents (QPainter* painter)
{
    // TODO make this method also applicable to the spline curve
    if (!m_tangents.empty ())
    {
	double halfLen = 30.;
	for (unsigned i = 0; i < m_points.size(); i++)
	{
	    QPointF first  = m_points[i] - halfLen * m_tangents[i]/ vectorLen(m_tangents[i]);
	    QPointF second = m_points[i] + halfLen * m_tangents[i]/ vectorLen(m_tangents[i]);
	    painter->drawLine (first, second);
	}
    }
}

void
IgTrack::calcDistances (vector<double>& distances)
{
    // determine distances for consecutive points, needed for parametrization
    double totalLength = 0.;
    for (unsigned i = 0; i < m_points.size () - 1; i++)
    {
	distances[i] = distance (m_points[i], m_points[i+1]);
	totalLength += distances[i];
    }
    m_totalCurveLength = totalLength;
}

void 
IgTrack::calcParametricPoints (vector<double>& distances, vector<double>& x, vector<double>& y, vector<double>& paramSteps)
{
    // this method separates the points' coordinates and calculates the (unit)
    // 'parametric coordinates' by using the euklidian distance and puts them
    // into paramSteps [0,1].
    unsigned n = m_points.size ();
    double sumOfDists = 0.;
    double tempArcLength = 0.;
	
    for (unsigned i = 0; i < n; i++)
    {
	tempArcLength = sumOfDists / m_totalCurveLength;
		
	x.push_back (m_points[i].x ());
	y.push_back (m_points[i].y ());
	paramSteps.push_back (tempArcLength);
		
	sumOfDists += distances[i];
    }
}

double
IgTrack::distance (const QPointF& a, const QPointF& b)
{
    // returns the euklidian distance between two points
    return sqrt (pow (b.x () - a.x (), 2) + pow (b.y () - a.y (), 2));
}


void 
IgTrack::tridiagonalSolve (const vector<double>& a, const vector<double>& b, 
                           vector<double>& c, vector<double>& d, vector<double>& x)
{
    // NOTE:
    // This is an implementation of the tridiagonal matrix alogrithm aka Thomas
    // algorithm. The matrix is an (n x n)-matrix and has three non-zero(?) central
    // diagonals. The lower diagonal is represented by the vector a, the middle
    // diagonal by vector b and the upper diagonal by vector c. Vector d is the
    // right-hand-side of the system Ax=b. The solution is written into the
    // vector x. Note that the vectors a, b and c have the same length, but the
    // first (of a) resp. the last (of c) position is not relevant.
    const unsigned n = x.size ();
    double id, denom;
	
    // Modify the coefficients.
    if (b[0] != 0.)
    {
	c[0] = c[0]/b[0];
	d[0] = d[0]/b[0];
    }
    else
    {
	cerr << "Division by zero! Check the matrix! Exiting..." << endl;
	return;
    }
	
    for (int i = 1; i != (int)n; i++)
    {
	denom = (b[i] - c[i - 1]*a[i]);
	if (denom != 0.)
	{
	    id = 1.0/denom;
	}
	else
	{
	    cerr << "Division by zero! Check the matrix! Exiting..." << endl;
	    return;
	}
	c[i] = c[i]*id; // Last value calculated is redundant.
	d[i] = (d[i] - a[i]*d[i - 1])*id;
    }

    // Now back substitute.
    x[n - 1] = d[n - 1];
    for (int i = n - 2; i != -1; i--)
    {
	x[i] = d[i] - c[i]*x[i + 1];
    }
}

void
IgTrack::determineCoeffs (vector<double>& param, vector<double>& a, vector<vector<double> >& coefficients)
{
    // this method calculates the spline equation's coefficients in a column major way: 
    // coefficients = [(a),(b),(c == _x),(d)]
    // a = [val0, val1, val2, val3, ...], same for b, c and d
    unsigned n = a.size ();
	
    vector<double> h; // 0 until (inclusive) n-2
    //vector<double>& a is: 0 until (inclusive) n-1
	
    // matrix equation variables
    vector<double> A_a (n, 0.);
    vector<double> A_b (n, 1.);
    vector<double> A_c (n, 0.);
    vector<double> _d (n, 0.);
    vector<double> _x (n, 0.);
	
    // parametric distance between point i and i=1
    for (unsigned i = 0; i < n-1; i++)
    {
	h.push_back (param[i+1] - param[i]);
    }

    // prepare the matrix' 3 diagonals
    for (unsigned i = 1; i < n-1; i++) // 0 until (inclusive) n-1
    {
	A_a[i] = h[i-1];
	A_b[i] = 2.* (h[i-1] + h[i]);
	A_c[i] = h[i];
    }

    // prepare the equation's right-hand-side
    for (unsigned i = 1; i < n-1; i++)
    {
	_d[i] = 3. * ((a[i+1] - a[i]) / h[i] - (a[i] - a[i-1]) / h[i-1]);
    }

    // solve Ax=d
    tridiagonalSolve (A_a, A_b, A_c, _d, _x);
	
    vector<double> b (n, 0.);
    vector<double> d (n, 0.);
	
    // calculate the missing coefficients, namely b and d (_d is not directly
    // related to d). We can take over a, and _x equals c already.
    for (int j = (int)n-2; j >= 0; j--)
    {
	b[j] = (a[j+1] - a[j]) / h[j] - h[j] * (_x[j+1] + 2. * _x[j])/3.;
	d[j] = (_x[j+1] - _x[j]) / (3. * h[j]);
    }

    coefficients.push_back (a);
    coefficients.push_back (b);
    coefficients.push_back (_x);
    coefficients.push_back (d);
}


void
IgTrack::getSplinesPath (QPainterPath& curvePath, vector<double>& coeffX,
			 vector<double>& coeffY, double intervalStart, 
			 double intervalEnd, double steps /* = 0.*/)
{
    // each interval between two consecutive points is interpolated by a parametric cubic
    // spline defined by two equations:
    //
    //    Sx(t) = a_x + b_x*(s - si) + c_x*(s - si)^2 + d_x*(s - si)^3
    //    Sy(t) = a_y + b_y*(s - si) + c_y*(s - si)^2 + d_y*(s - si)^3
    //
    // The splines of all intervals collected in a path.
	
    double delta = 1.;
    // use the user set step length otherwise take the inverse of the curve's length
    if (steps == 0.)
    {
	delta = 1 / m_totalCurveLength;
    }
    else
    {
	delta = (intervalEnd - intervalStart) / steps;
    }

    double k_j = 0.;
    double value_x = 0.;
    double value_y = 0.;
	
    double& a_x = coeffX[0];
    double& b_x = coeffX[1];
    double& c_x = coeffX[2];
    double& d_x = coeffX[3];
	
    double& a_y = coeffY[0];
    double& b_y = coeffY[1];
    double& c_y = coeffY[2];
    double& d_y = coeffY[3];
	
    // first move to the first point
    curvePath.moveTo (a_x, a_y);
	
    // now start to draw the curve by using lines, to get a smoother curve increase 'steps'
    for (double s = intervalStart/* + delta*/; s < intervalEnd + delta/2.; s += delta)
    {
	k_j = s - intervalStart;
	value_x = a_x + b_x * k_j + c_x * pow (k_j, 2) + d_x * pow (k_j, 3);
	value_y = a_y + b_y * k_j + c_y * pow (k_j, 2) + d_y * pow (k_j, 3);
	curvePath.lineTo (value_x, value_y);
    }
}

double 
IgTrack::vectorLen (const QPointF& a)
{
    // get the length of this vector
    return sqrt (a.x () * a.x () + a.y () * a.y ());
}


void
IgTrack::getHermitePath (QPainterPath& curvePath, unsigned index, double steps /* = 0.*/)
{
    // this method computes the path of the curve using Cubic Hermite Splines
    // between the track points. The difference to the SPLINE algorithm is that
    // tangents must be given to be able to use it. Especially the tangents
    // should not be normalized, because its length is determining how
    // "strongly" the curve is bent. If the drawn curve is somewhat unsmooth,
    // the user can define a suitable steps size for sampling. The default
    // sampling is kind of dynamic and uses the inverse of the distance between
    // two points. The idea behind that is, that the distance between two
    // points is given in pixel, so a sampling rate smaller than one pixel is
    // unnecessary and does not improve the result. Taking the inverse of the
    // distance gives us the x-th part of a interval which is 1 in length ->
    // used for t [0;1]
    //
    // Used formula between 2 points:
    // t = [0;1]
    // a = (2t^3 - 3t^2 + 1)
    // b = (t^3 - 2t^2 + t)
    // c = (-2t^3 + 3t^2)
    // d = (t^3 - t^2)
    // p0 = point0 (_x = x-component, _y = y-component)
    // p1 = point1
    // m0 = tangent vector at point0
    // m1 = tangent vector at point1
    //
    // Hx(t) = a*p0_x + b*m0_x + c*p1_x + d*m1_x
    // Hy(t) = a*p0_y + b*m0_y + c*p1_y + d*m1_y

    double delta [2];
    double start [2]   = {m_points[index].x(),     m_points[index].y()};
    double end   [2]   = {m_points[index+1].x(),   m_points[index+1].y()};
    double start_t [2] = {m_tangents[index].x(),   m_tangents[index].y()};
    double end_t [2]   = {m_tangents[index+1].x(), m_tangents[index+1].y()};
    vector<vector<double> > value;
	
    // use the user set step length otherwise take the inverse of the distance
    if (steps == 0.)
    {
	delta[0] = 1. / distance(m_points[index], m_points[index+1]);
	delta[1] = 1. / distance(m_points[index], m_points[index+1]);
    }
    else
    {
	delta[0] = 1. / steps;
	delta[1] = 1. / steps;
    }
	
    for (unsigned dim = 0; dim < 2; dim++)
    {
	vector<double> temp_val;
	for (double t = 0.; t < 1.0 + delta[dim]/2.; t += delta[dim])
	{
	    double t_2 = pow (t, 2.);
	    double t_3 = t_2 * t;
			
	    double a = (2.*t_3 - 3.*t_2 + 1.);
	    double b = (t_3 - 2.*t_2 + t);
	    double c = (-2.*t_3 + 3.*t_2);
	    double d = (t_3 - t_2);
			
	    temp_val.push_back (a*start[dim] + b*start_t[dim] + c*end[dim] + d*end_t[dim]);
	}
	value.push_back (temp_val);
    }
	
    for (unsigned i = 0; i < value[0].size(); i++)
    {
	curvePath.lineTo (value[0][i], value[1][i]);
    }
}

void
IgTrack::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //    bool showBoundingBox = false;
    painter->setPen (pen ());
    
    if (m_showPoints)        drawPoints (painter);
    if (m_showInterpolation) drawCurve (painter);
    if (m_showTangents)      drawTangents (painter);
    if (m_showBBox) painter->drawRect (calcBoundingRect ());
}
