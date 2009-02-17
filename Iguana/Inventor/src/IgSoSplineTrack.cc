//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <cmath>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
namespace {
#ifdef TGS_VERSION
    static const float maxNurbAngle =  M_PI / 2.; // was M_PI/2
#else
    static const float maxNurbAngle = M_PI / 6.;
#endif
}
//<<<<<< PRIVATE TYPES                                                  >>>>>>
namespace { //blank namespace is eqivalent to file scope static
    // rootFunction evaluates the formula for the equation defining
    // the opening angle of a helical curve in terms of the invariants.
    // The argument x represents
    // the possible values of the angle.

    class RootFunction : public std::unary_function<float,float>
    {
    public:
	RootFunction(float paramA, float paramC, float paramF) :
	  a(paramA), c(paramC), f(paramF)
	  {};
	  float operator()(const float x) const
	  {
	      double denom = -2* (1-cos (x))* (1-a) +x*x* (cos (x)-a); 
	      double numerator = c * (cos (x)-a);
	      double radical = numerator / denom; //might as well return INF if denom=0
	      if (radical > 0) {
		  radical = sqrt (radical);}  //return Re part
	      else {
		  radical = 0; //-sqrt (-radical); //return -Im part
	      }

	      return static_cast<float> (f + radical * (1-a)* (-2+2*cos (x)+x*sin (x))/ (cos (x)-1));
	  }
    private:
	const double a, c, f;
    };
}

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const float M_SQRT3_2 =  0.8660254037844F; // sqrt (3)/2 for sin (60 degrees)
const int IgSoSplineTrack::NORDER = 3;

SO_KIT_SOURCE (IgSoSplineTrack);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoSplineTrack::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoSplineTrack, IgSoShapeKit, "IgSoShapeKit"); }

IgSoSplineTrack::IgSoSplineTrack (void)
{
    SO_KIT_CONSTRUCTOR (IgSoSplineTrack);

    SO_KIT_ADD_FIELD (points, (SbVec3f (0, 0, 0)));
    SO_KIT_ADD_FIELD (tangents, (SbVec3f (0, 0, 0)));

    SO_KIT_ADD_CATALOG_ENTRY (coordinates, SoCoordinate4, FALSE, separator, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (curve, SoNurbsCurve, FALSE, separator,\x0, TRUE);

    SO_KIT_INIT_INSTANCE ();

    //      We make the default shape here rather than with the SO_NODE_ADD_FIELD calls

    //  straight line
    SbVec3f defPoints[2] = {SbVec3f (0.F,0.F,0.F),
	SbVec3f (0.F,1.F,0.F)};
    SbVec3f defTangents[2] = {SbVec3f (1.F,0.F,0.F),
	SbVec3f (1.F,0.F,0.F)};
    points.setValues (0, 2, defPoints);
    tangents.setValues (0, 2, defTangents);

    setUpConnections (true, true);
}

void
IgSoSplineTrack::refresh (void)
{
    int npts = points.getNum ();  
    if (npts <= 1) 
    {     
	setPart ("coordinates", NULL);
	setPart ("curve", NULL);
	return;
    }

    SoCoordinate4 *coords = new SoCoordinate4;
    SoNurbsCurve  *ncurve = new SoNurbsCurve;

    // Start by assuming that we won't have to insert any extra points
    std::vector <SbVec4f> controlPoints (2 * npts - 1);
    std::vector <float> knotVals (2 * npts - 1 + NORDER);

    int ik = 0; //pointer for knots
    int ipi = 0; // in pointer for control points 
    int ik_count = 0; //current value for knot
    int ipo = 0;  // out pointer for control points
    // the end points require an extra knot
    knotVals[ik++] = static_cast<float>(ik_count);
    // Now do the loop over points, converting to control points
    while (ipi < (npts - 1)) 
    {
	knotVals[ik++] = static_cast<float>(ik_count);
	controlPoints[ipo++] = SbVec4f ( points[ipi][0], points[ipi][1], points[ipi][2], 1.F);
	//      Calculate and insert the mid control point(s)
	midpoint (points[ipi], points[ipi+1], tangents[ipi], tangents[ipi+1], ik, ik_count, ipo, controlPoints, knotVals);
	ipi++;
    }
    // Put in the last point
    controlPoints[ipo++] = SbVec4f ( points[ipi][0], points[ipi][1], points[ipi][2],1.F);
    knotVals[ik++] = static_cast<float>(ik_count);
    knotVals[ik++] = static_cast<float>(ik_count);
    knotVals[ik++] = static_cast<float>(ik_count);

    // Now stuff it all in the NURB
    coords->point.setValues (0,ipo,&(controlPoints[0]));
    ncurve->knotVector.setValues (0,ik, &(knotVals[0]));
    ncurve->numControlPoints.setValue (ipo);

    setPart ("coordinates", coords);
    setPart ("curve", ncurve);
}

void
IgSoSplineTrack::midpoint (const SbVec3f pa, const SbVec3f pb, const SbVec3f ta, const SbVec3f tb,
			   int &ik, int &ik_count, int &ipo, std::vector<SbVec4f> &controlPoints, std::vector<float> &knotVals)
{
    // generate the control points between two given points: pa and pb, and 
    // two tangents: ta and tb.
    // return the zero vector if there's a problem

    const float eps = 0.000001F;

    //  Will solve for an approximate helix using the available invariants.
    //      Parametrize result as h[phi] = rho (cos[phi], sin[phi], zeta*phi)
    float paramA = tb.dot (ta);
    float paramC = (pb-pa).dot (pb-pa);
    float paramD = tb.dot (ta.cross (pb-pa));

    // these values will be used if we bail out...
    SbVec3f midpt3s ((pa+pb)/2.F);
    SbVec4f midpt4s (midpt3s[0],midpt3s[1],midpt3s[2],1.F);
    //      Tests for pathological cases
    //      Straight Line   
    if (std::abs (paramC)<eps || (std::abs (paramA-1) < eps && std::abs (paramD)/paramC < eps)) {
	controlPoints[ipo++] = midpt4s;
	knotVals[ik++] = static_cast<float>(ik_count++);
	return;
    }

    //      Linearly dependent but not straight
    SbVec3f perp = ta.cross (tb);
    if (std::abs (perp.dot (perp)) < eps)
    {
	controlPoints[ipo++] = midpt4s;
	knotVals[ik++] = static_cast<float>(ik_count++);
	return;
    }

    float zeta = 0.F;
    float delta = 0.F;
    float rho = 0.F;
    //      indeterminate solution in the root finder: add a tiny bit to paramD, enough to
    //      kick it out of danger but not enough to perturb the solution much.
    if (std::abs (paramD)<eps) 
    {
	// either delta = 0 or zeta = 0 (or both)
	// since delta can't be zero for a good measurement,
	zeta = 0.F;
	delta = std::acos(paramA);
	rho = std::sqrt(paramC/2.F/(1.F-paramA));
	paramD = -1.5F*eps;
    }
    else
    {
	//  try for a solution
	bool err = true;
	int paramSign = 1;

	delta = findRoot (RootFunction(paramA, paramC, -paramD), 0.01F, static_cast< float > (M_PI), eps, err);
	if (err) {
	    paramSign = -1;
	    delta = findRoot (RootFunction(paramA, paramC, paramD), 0.01F, static_cast< float > (M_PI), eps, err);
	    if (err) 
	    {
#if 0 //DEBUG
		std::cout << "Problem in SoSplineTrack. Can't find root. " << std::endl;
		float x,y,z;
		pa.getValue (x,y,z);
		std::cout << "pa: " << x << ", " << y << ", " << z << std::endl;
		pb.getValue (x,y,z);
		std::cout << "pb: " << x << ", " << y << ", " << z << std::endl;
		ta.getValue (x,y,z);
		std::cout << "ta: " << x << ", " << y << ", " << z << std::endl;
		tb.getValue (x,y,z);
		std::cout << "tb: " << x << ", " << y << ", " << z << std::endl;
#endif //DEBUG                  
		controlPoints[ipo++] = midpt4s;
		return;
	    }
	}
	zeta = paramSign * sqrt (std::abs ((paramA-cos (delta))/ (1.0F-paramA)));
	rho = sqrt (paramC/ (2+delta*delta*zeta*zeta-2*cos (delta)));
    }
    // Now calculate ta, tb, pa, pb for parametrized curve
    SbVec3f taMP = SbVec3f ( 0.f, rho, rho*zeta);
    taMP.normalize();
    SbVec3f paMP = SbVec3f ( rho, 0.0f, 0.f );
    SbVec3f tbMP = SbVec3f( -rho * sin(delta), rho * cos(delta), rho * zeta );
    tbMP.normalize();
    SbVec3f tc = ta.cross(tb);
    tc.normalize();
    SbVec3f tcMP = taMP.cross(tbMP);
    tcMP.normalize();

    // rotation to go from taMP, tbMP frame to ta, tb

    SbMatrix mMP(taMP[0], tbMP[0], tcMP[0], 0.f,
	taMP[1], tbMP[1], tcMP[1], 0.f,
	taMP[2], tbMP[2], tcMP[2], 0.f,
	0.f,  0.f,  0.f,  1.f);

    SbMatrix m(ta[0], tb[0], tc[0], 0.f,
	ta[1], tb[1], tc[1], 0.f,
	ta[2], tb[2], tc[2], 0.f,
	0.f,   0.f,   0.f,   1.f);

    SbMatrix mm = m.multRight(mMP.inverse()).transpose(); // OIV stores row-major but GL uses column-major
    // so use as transpose in reverse order

    // correct order to transform pts in MP back to lab is:
    // subtract paMP; rotate to align with ta, tb; add pa
    SbMatrix xform;
    xform.setTranslate(-paMP);
    xform = xform.multRight(mm);
    SbMatrix transBack;
    transBack.setTranslate(pa);
    xform = xform.multRight(transBack);

    // if we exceed the maximum extent well-rendered by the base nurb library, add more divisions
    const int ndiv = 1 + static_cast<int>(std::floor(std::abs(delta)/maxNurbAngle));

    float deltan = delta/ndiv;
    float wt = std::abs (cos (deltan/2) );
#if 0 //DEBUG
    if (wt < eps) 
    {
	std::cout << "Problem in SoSplineTrack. Wt = " << wt << std::endl;
	std::cout << "    params: " << paramA <<","<< paramC << "," << paramD << std::endl;
    }
#endif //DEBUG  

    // adjust size of array to accomodate additional inserted points
    if (ndiv > 1)
    {
	controlPoints.resize( controlPoints.size()+2*(ndiv-1) );
	knotVals.resize( knotVals.size()+2*(ndiv-1) );
    }
    //  Calculate and insert points
    float angle=0.F;
    for ( int i=0; i < ndiv; i++, angle += deltan )
    {
	if (i!=0) 
	{
	    SbVec3f endpt ( rho * std::cos(angle), rho * std::sin(angle), rho * zeta * (angle) );
	    xform.multVecMatrix ( endpt, endpt );
	    SbVec4f endpt4 ( endpt[0], endpt[1], endpt[2], 1.F );
	    knotVals[ik++] = static_cast<float>(ik_count);
	    controlPoints[ipo++] = endpt4;
	}
	SbVec3f midpt3_0 ( rho * std::cos (angle+deltan/2.0F), rho * std::sin (angle+deltan/2.0F), rho * zeta*(angle+deltan/2.0F));
	SbVec3f midpt3 (midpt3_0[0]/wt, midpt3_0[1]/wt, midpt3_0[2]);

	xform.multVecMatrix (midpt3,midpt3);
	SbVec4f midpt (midpt3[0],midpt3[1],midpt3[2],1.F); //convert from Vec3 to Vec4
	midpt *= wt; //and set the correct overall weight
	controlPoints[ipo++] = midpt;
	knotVals[ik++] = static_cast<float>(ik_count++);
    }

}

float
IgSoSplineTrack::findRoot (const RootFunction func, float x1, float x2, float xacc, bool &err)
{
    // Find a root using bisection (see Numerical Recipies for complete documentation)
    err = false;
    const int JMAX = 40;

    func(x1);
    float f= func(x1);
    float fmid= func(x2);
    //      Might want to insert a check here to see if either of the endpoints or the midpoint
    //      are within xacc of 0...
    if (f*fmid >=0.0) {
	err = true;
#if 0 //DEBUG
	std::cout << "SoSplineTrack f*fmid >= 0" << std::endl;
	std::cout << "  f="<< f << "  fmid=" << fmid << std::endl;
#endif //DEBUG
	return 0.0;}
    else {
	float dx, xmid;
	float rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2);
	for (int j=1; j<JMAX; j++) {
	    fmid= func(xmid=rtb + (dx *= 0.5));
	    if (fmid <= 0.0) rtb=xmid;
	    if (std::abs (dx) < xacc || fmid == 0.0) return rtb;
	}
	err = true;
#if 0 //DEBUG
	std::cout << "SoSplineTrack: findRoot failed to converge" << std::endl;
	std::cout << "  f="<< f << "  fmid=" << fmid << "  rtb=" << rtb << std::endl;
#endif //DEBUG
	return 0.0;
    }
}
